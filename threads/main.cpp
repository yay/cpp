#include <iostream>
#include <thread>
#include <chrono>

static bool isFinished = false;

void printThreadId() {
    std::cout << "Started thread id=" << std::this_thread::get_id() << std::endl;
}

void doWork() {
    using namespace std::literals::chrono_literals;

    printThreadId();
    while (!isFinished) {
        std::cout << "Working...\n";
        std::this_thread::sleep_for(1s);
    }
}

int main(int, char**) {
    printThreadId();

    // spawn worker thread that starts working immediately
    std::thread worker(doWork);

    std::thread slacker([](){
        std::cout << "Whazzzup?!" << std::endl;
    });

    // in this (main) thread
    std::cin.get();    // wait till we hit Enter
    isFinished = true; // set flag to true, when Enter is hit
    worker.join();     // and wait for the worker thread to finish
    slacker.join();

    std::cout << "Finished!\n";
}
