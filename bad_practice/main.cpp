#include <iostream>
#include <string>

int stackStrings() {
    std::string s1 = "Hello, my name is Vitaly."; // stack allocated
    std::string s2 = s1; // a copy of the whole string is made, including character data
    s2[1] = 'A';
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    return s1.length() + s2.length(); // `length` method is accessed directly
}

int heapStrings() {
    std::string *s1 = new std::string("Hello, my name is Vitaly.");
    std::string *s2 = s1; // Only the stack struct (data pointer, length, capacity) is copied.
    // s2[1] = 'A'; // works! but changes the memory at offset 1 from the string pointer.
    (*s2)[1] = 'A'; //
    std::cout << *s1 << std::endl;
    std::cout << *s2 << std::endl;
    return s1->length() + s2->length();
}

int main(int, char**) {
    stackStrings();
    heapStrings();
    return 0;
}

void pointers() {
    int* p = 0;     // !!!
    int* pp = NULL; // !!!

    int* ppp = nullptr;
}

void unsgn() {
    unsigned int ui = 4;
    for (int i = 0; i < ui; i++) { // !!! use `unsigned` for bit manipulation only
        std::cout << i << std::endl;
    }
}

void declaration() {
    int column, row, index = 5; // !!!
    printf("%d %d %d\n", column, row, index); // only index is 5

    int* x, y; // !!! only x is int pointer, y is simply int
    int *a, *b; // this solves the issue, but can look odd:
                // see version1 and version2 functions

    // a better rule would be `one name per declaration`:
    int* u;
    int* v;
}

// - odd to have `*` on the `version1` side
// - odd to have a space between `int` and `*` when the variable name is missing
int *version1(int *, float *) { // !!!
    int *a = new int{42};
    return a;
}

int* version2(int*, float*) {
    int* a = new int{42};
    return a;
}
