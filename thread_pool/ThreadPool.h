// https://github.com/progschj/ThreadPool

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <type_traits>

class ThreadPool {
public:
    ThreadPool(size_t);
    
    template<class F, class... Args>
    auto enqueue(F&& f, Args&& ... args)->std::future<typename std::invoke_result<F, Args...>::type>;
    // https://en.cppreference.com/w/cpp/types/result_of
    ~ThreadPool();
    
private:
    // Need to keep track of threads, so we can join them.
    std::vector<std::thread> workers;
    // The task queue.
    std::queue<std::function<void()> > tasks;
    
    // Synchronization.
    std::mutex queue_mutex;
    std::condition_variable condition; // https://en.cppreference.com/w/cpp/thread/condition_variable
    bool stop;
};

// The constructor just launches some amount of workers.
inline ThreadPool::ThreadPool(size_t thread_count) : stop(false) {
    for (size_t i = 0; i < thread_count; i++) {
        workers.emplace_back([this] {
            for (;;) {
                std::function < void() > task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

// Add new work item to the pool.
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&& ... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;
    
    auto task_type = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto task = std::make_shared<std::packaged_task<return_type()>>(task_type);
    
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        
        // Don't allow enqueueing after stopping the pool.
        if (stop) {
            throw std::runtime_error("Enqueue on stopped ThreadPool.");
        }
        
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

// The destructor joins all threads.
inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
}

#endif
