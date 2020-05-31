#include <iostream>
#include <filesystem>

template <int N>
struct Factorial {
    enum { value = N * Factorial<N-1>::value };
};

template<>
struct Factorial<0> {
    enum { value = 1 };
};

void foo() {
    // evaluates at compile time
    int f4 = Factorial<4>::value;
    int f0 = Factorial<0>::value;

    std::cout << f4 << std::endl; // 24
    std::cout << f0 << std::endl; // 1
}

int main(int, char**) {
    foo();

	return 0;
}
