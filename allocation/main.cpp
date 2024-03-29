#include <print>

void *operator new(std::size_t n)
{
    std::println("Allocating {} bytes", n);
    return malloc(n);
}

void operator delete(void *p) throw()
{
    free(p);
}

int main()
{
    // Heap allocation starts for strings that exceed an implementation-dependent size:
    // > 14 for MSVC and GCC
    // > 22 for Clang
    std::string small_string_optimization = "1234567890123456789012"; // no heap allocation
    std::string big_string = "12345678901234567890123";               // Allocating 32 bytes

    return 0;
}