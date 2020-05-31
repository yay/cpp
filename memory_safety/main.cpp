#include <iostream>
#include <vector>
#include <string>

int main(int, char**) {
    std::vector<std::string> v;
    v.push_back("Hello");
    std::string &s = v[0];
    v.push_back(", world!"); // 1) vector allocates enough space to hold two strings
                             // 2) copies the first string over to it
                             // 3) adds the second string
                             // reference s is no longer valid
    std::cout << s << std::endl; // segfault or undefined behavior
}
