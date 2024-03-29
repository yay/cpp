#include <filesystem>
#include <print>

template <int N>
struct Factorial
{
    enum
    {
        value = N * Factorial<N - 1>::value
    };
};

template <>
struct Factorial<0>
{
    enum
    {
        value = 1
    };
};

void foo()
{
    // evaluates at compile time
    int f4 = Factorial<4>::value;
    int f0 = Factorial<0>::value;

    std::println("{}", f4); // 24
    std::println("{}", f0); // 1
}

int main(int, char **)
{
    foo();

    return 0;
}
