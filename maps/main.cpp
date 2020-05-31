#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "limits.h"

void basics() {
    // inferred type
    auto map = std::map<std::string, std::string> {
            {"name", "Vitaly"},
            {"country", "Russia"}
    };

    // explicit type
    std::__1::map<
            std::__1::basic_string<char>,
            std::__1::basic_string<char>,
            std::__1::less<std::__1::basic_string<char> >,
            std::__1::allocator<std::__1::pair<const std::__1::basic_string<char>, std::__1::basic_string<char> > >
    > &explicitMap = map;

    // explicit type
    std::map<std::string, std::string> &emptyMap = map;

    map.insert({"age", "33"});

    auto name = map["name"];
    std::cout << name << "\n";

    map["surname"] = "Kravchenko";

    map.erase(map.find("age")); // remove key/value
}

void iteration() {
    std::map<std::string, std::vector<int>> map {
        {"Prime", {2, 3, 5, 7, 11, 13}},
        {"Fibonacci", {1, 1, 2, 3, 5, 8}},
        {"Square", {1, 4, 9, 16, 25}}
    };

    map["Random"] = {3, -3, -27, -4, 5};

    for (auto &numbers : map) {
        int largest {INT_MIN};
        for (auto n : numbers.second) {
            if (n > largest && n % 2 == 0) {
                largest = n;
            }
        }
        printf("%s: %i\n", numbers.first.data(), largest);
    }
}

int main(int, char**) {
    basics();
    iteration();
}