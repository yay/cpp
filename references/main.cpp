#include <iostream>

int main(int, char**) {
    int i {5};
    int *pi = new int;
    *pi = 6; // need to dereference the pointer first to change the value
    std::cout << "pi value: " << *pi << std::endl; // 6
    std::cout << "pi address before delete: " << pi << std::endl;
    delete pi; // the memory is marked as freed
    std::cout << "pi address after delete: " << pi << std::endl; // same as before delete
    std::cout << "pi value after delete: " << *pi << std::endl;  // most certainly still 6
    // but doesn't have to be, could be overwritten, since it was freed
    // We are not supposed to be using `pi` after we freed it ("use after free" or "dangling pointer").
    pi = nullptr;
    std::cout << "pi address after nullptr: " << pi << std::endl;
    // std::cout << "pi value after nullptr: " << *pi << std::endl; // segmentation fault

    std::cout << "-------------" << std::endl;

    pi = new int {7};
    int *ppi = pi; // creating another pointer that points to the int value 7
    int &ri = *pi; // creating a reference that points to the int value 7

    // Regardless of which of the three assignments are used,
    // the code below them acts the same:
    // *pi = 8;
    // *ppi = 8;
    ri = 8; // no need to dereference the reference to change the value,
            // it happens automatically under the hood

    // All three point to the same address in memory:
    std::cout << "pi address: " << pi << std::endl;
    std::cout << "ppi address: " << ppi << std::endl;
    std::cout << "ri address: " << &ri << std::endl;

    std::cout << "pi value: " << *pi << std::endl;   // 8
    std::cout << "ppi value: " << *ppi << std::endl; // 8
    std::cout << "ri value: " << ri << std::endl;    // 8

    delete pi; // the memory is marked as freed

    // the reference is still pointing to the same memory address
    std::cout << "ri address after delete: " << &ri << std::endl; // same as before delete
    // "Use after free":
    std::cout << "ri value after delete: " << ri << std::endl;  // 8 (most likely)
    std::cout << "pi value after delete: " << *pi << std::endl; // 8 (most likely)

    // ri = nullptr; // can't do this with references

    float &rf = *new float {9}; // use dereferencing to point `rf` to the actual value
    std::cout << "rf value: " << rf << std::endl;
    delete &rf;
}
