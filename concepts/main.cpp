#include <concepts>
#include <cstddef>
#include <iostream>
#include <string>

template <class From, class To>
concept convertible_to =
    std::is_convertible_v<From, To> &&requires(std::add_rvalue_reference_t<From> (&f)()) {
        static_cast<To>(f());
    };

template <typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> convertible_to<std::size_t>;
};

template<Hashable T>
void f(T) {}

void test_hashable() {
    using namespace std::literals;

    f("abc"s);

    struct meow {};
    // f(meow{}); // does not satisfy Hashable
}

int main(int, char **) {
    test_hashable();
}
