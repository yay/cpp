#include <iostream>
#include <type_traits>

// https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp

// Type traits are used to inspect and transform the properties of types.
// This is useful in conditional compilation, where you instruct the compiler
// to pick the right path according to the type of input.
// Type traits can also apply some transformation to a type. For example, given T,
// you can add/remove the const specifier, the reference or the pointer, or turn it
// into a signed/unsigned type and many other operations.

// Type inspection.

class Class {};

void test_is_floating_point() {
    std::cout << std::is_floating_point<Class>::value << std::endl;
    std::cout << std::is_floating_point<float>::value << std::endl;
    std::cout << std::is_floating_point<int>::value << std::endl;
}

// Under the hood the compiler generates three different structs:
struct is_floating_point_Class {
    static const bool value = false;
};

struct is_floating_point_float {
    static const bool value = true;
};

struct is_floating_point_int {
    static const bool value = false;
};

void algorithm_signed(int i) {
    std::cout << "Signed version used" << std::endl;
}

void algorithm_unsigned(unsigned u) {
    std::cout << "Unsigned version used" << std::endl;
}

template <class T>
void algorithm(T t) {
    if constexpr (std::is_signed<T>::value) {
        algorithm_signed(t);
    } else if constexpr (std::is_unsigned<T>::value) {
        algorithm_unsigned(t);
    } else {
        static_assert(std::is_signed<T>::value || std::is_unsigned<T>::value, "Must be signed or unsigned!");
    }
}

// Type transformation.

// For example, std::move turns a type T into an rvalue reference T&&.
// Transformations like this one are widespread across the whole Standard Library,
// often used to optimize how function parameters flow across nested template function calls.

// Aliases since C++14:
// std::is_signed<T>::value;     --->   std::is_signed_v<T>;
// std::remove_const<T>::type;   --->   std::remove_const_t<T>;

int main(int, char**) {
    test_is_floating_point();
    algorithm(-3);
    algorithm(5u);
    // algorithm("string");
}
