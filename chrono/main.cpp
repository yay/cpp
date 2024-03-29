#include <iostream>
#include <print>
#include <chrono>
#include <thread>
#include <future>
#include <ctime>

// http://en.cppreference.com/w/cpp/chrono/c/time
// https://habr.com/post/324984/

using namespace std::chrono;

int find_answer()
{
    std::this_thread::sleep_for(2s);
    return 42;
}

std::future<int> f = std::async(find_answer);

int main(int, char **)
{
    if (f.wait_for(2500ms) == std::future_status::ready)
        std::println("Answer is: {}", f.get());
    else
        std::println("Can't wait anymore!");

    std::time_t t = std::time(nullptr);
    std::println("Seconds since the epoch: {}", t);
    // Note: std::asctime and std::localtime are not thread-safe (have static internal storage).
    std::println("{}", std::asctime(std::localtime(&t)));

    // auto now = system_clock::now();
}
