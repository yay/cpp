#include <iostream>
#include <vector>
void helloWorld(int a) {
    std::cout << "Hello World! Value: " << a << std::endl;
}

void printValue(int value) {
    std::cout << "Value: " << value << std::endl;
}

void forEach(const std::vector<int>& values, void(*func)(int)) {
    for (int value : values) {
        func(value);
    }
}

void printVector() {
    std::vector<int> values { 1, 5, 4, 2, 3 };
    forEach(values, printValue);
    std::cout << std::endl;
    forEach(values, [](int value) { std::cout << "Value: " << value << std::endl; });
}

int main(int, char**) {
    typedef void(*HelloWorldFunction)(int);

    void (*fun)(int); // the actual type is 'void (*)()',
                      // but we need to give it a name, in this case 'fun'

    std::cout << (void *)helloWorld << std::endl; // prints address
    // std::cout << helloWorld << std::endl;      // error:
    // Address of a function will always evaluate to 'true' [-Wpointer-bool-conversion]
    std::cout << &helloWorld << std::endl;        // prints 1
    std::cout << (void*)&helloWorld << std::endl; // prints address

    auto function = &helloWorld; // the ampersand is not required
    fun = helloWorld;
    HelloWorldFunction func = helloWorld;

    if (function) {
        std::cout << "'function' is assigned\n";
    }

    helloWorld(8);
    function(8);
    fun(8);
    func(8);

    if (function == helloWorld) {
        std::cout << "Equal\n";
    }
    if (fun == helloWorld) {
        std::cout << "Equal\n";
    }
    if (func == helloWorld) {
        std::cout << "Equal\n";
    }

    printVector();
}

