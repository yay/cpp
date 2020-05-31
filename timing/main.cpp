#include <iostream>
#include <chrono>
#include <thread>

long long measureTimeMillis(void (*fn)()) {
    auto start = std::chrono::high_resolution_clock::now();
    fn();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

struct Timer {
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        float ms = duration.count() * 1000.0f;
        std::cout << "Timer took " << ms << "ms" << std::endl;
    }
};

int main() {
    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(1s);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    printf("Duration: %fs\n", duration.count());

    auto elapsed = measureTimeMillis([]() {
        std::this_thread::sleep_for(1s);
    });
    std::cout << "Duration: " << elapsed << "ms" << std::endl;

    {
        Timer timer;
        std::this_thread::sleep_for(1s);
    }
}
