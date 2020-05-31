#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <ctime>

// http://en.cppreference.com/w/cpp/chrono/c/time
// https://habr.com/post/324984/

using namespace std::chrono;

int find_answer() {
    std::this_thread::sleep_for(5s);
    return 42;
}

std::future<int> f = std::async(find_answer);

int main(int, char**) {
    if (f.wait_for(2500ms) == std::future_status::ready)
        std::cout << "Answer is: " << f.get() << "\n";
    else
        std::cout << "Can't wait anymore!\n";


    std::time_t result = std::time(nullptr);
    std::cout << std::asctime(std::localtime(&result))
              << result << " seconds since the Epoch\n";

    // auto now = system_clock::now();
}
