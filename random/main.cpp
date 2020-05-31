#include <iostream>
#include <thread>
#include <ctime>

int main(int, char**) {
    srand(time(NULL));

    const int n = 1'000'000'000;
    int* intArray = new int[n];

    for (int i = 0; i < n; i++) {
        intArray[i] = rand();
    }

    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(5s);

    delete[] intArray;
}
