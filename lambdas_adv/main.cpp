#include <iostream>
#include <functional>
#include <array>
#include <cstdlib> // for malloc() and free()

// On clang++ the size of all std::function is always 48 bytes (used to be 32 as of early 2016).
// It uses what is called `small size optimization`, much like std::string does on many
// implementations. This basically means that for small objects std::function can keep them
// as part of its memory, but for bigger objects it defers to dynamic memory allocation.

// Replace operator new and delete to log allocations.

void *operator new(std::size_t n) {
    std::cout << "Allocating " << n << " bytes" << std::endl;
    return malloc(n);
}

void operator delete(void *p) throw() {
    free(p);
}

int main() {
    std::array<char, 24> arr1;
    auto lambda1 = [arr1](){};
    std::cout << "Assigning lambda1 of size " << sizeof(lambda1) << std::endl;
    std::function<void()> f1 = lambda1;

    std::array<char, 25> arr2;
    auto lambda2 = [arr2](){};
    std::cout << "Assigning lambda2 of size " << sizeof(lambda2) << std::endl;
    std::function<void()> f2 = lambda2;

    // Output:
    // Assigning lambda1 of size 24
    // Assigning lambda2 of size 25
    // Allocating 40 bytes

    // 24 bytes is the threshold  beyond which std::function reverts to dynamic allocation
    // (on clang).
}
