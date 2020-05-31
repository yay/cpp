#include <iostream>

// Never ever do "using namespace" in a header file.

/*

Compilation steps:

1. Preprocessing (#include, #define, #if, #ifdef, #pragma)

*/

// #include can include .h files, .cpp files, files with other extensions,
// files with no extensions, files with text and no code, whatever
#include "2"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    std::cout << multiply(5, 3) << "\n";
#include "endBrace"
