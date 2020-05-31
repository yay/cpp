#include <iostream>
#include <stdlib.h>

void cpp() {
    auto path = std::getenv("PATH");
    auto home = std::getenv("HOME");

    std::cout << "PATH: " << path << std::endl;
    std::cout << "HOME: " << home << std::endl;
}

void c() {
    char* path = getenv("PATH");
    char* home = getenv("HOME");

    printf("PATH: %s\nHOME: %s\n", path, home);
}

int main() {
    cpp();
    c();

    return 0;
}