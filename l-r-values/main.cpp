#include <iostream>

auto getRvalue() -> int { return 10; }

auto getLvalueReference() -> int & {
    static int value = 10;
    return value;
}

// It's best to make the name a const reference, so that the function
// could take both lvalue AND rvalue references as parameters.
// As it is, the function only accepts lvalue references.
void print_lvalue_name(std::string &name) { std::cout << name << std::endl; }

// This function only accepts rvalue references.
void print_rvalue_name(std::string &&name) { std::cout << name << std::endl; }

void print_name(std::string &name) {
    std::cout << "lvalue: " << name << std::endl;
}

// An overloaded function like this gives us the ability to detect temporary
// values and do something with them. This is very useful for move semantics.
void print_name(std::string &&name) {
    std::cout << "rvalue: " << name << std::endl;
}

auto main() -> int {
    // i is an lvalue
    // 10 is an rvalue
    // can't assign to a temporary value 10
    int i = 10;
    int j = getRvalue();
    getLvalueReference() = 5;
    std::cout << getLvalueReference() << std::endl; // 5

    // can't take an lvalue reference from an rvalue:
    // int &a = 10; // error: initial value of reference to non-const must be an
    // lvalue
    const int &a =
        10; // valid, the compiler is probably creating a temp var to put 10
            // into under the hood, then gets a reference of that
    std::string first_name = "Vitaly";
    std::string last_name = "Kravchenko";
    // print_lvalue_name(first_name + last_name); // error: first_name +
    // last_name is an rvalue
    print_rvalue_name(first_name + last_name); // works
    // print_rvalue_name(first_name); // error: an rvalue reference cannot be
    // bound to an lvalue

    print_name(first_name);             // lvalue: Vitaly
    print_name(first_name + last_name); // rvalue: VitalyKravchenko
    print_name("hello");                // rvalue: hello
}