#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int, char**) {
    std::vector<int> values{1, 2, 3, 4, 5};

    for (int i = 0; i < values.size(); i++) {
        std::cout << values[i] << std::endl;
    }

    for (int value : values) {
        std::cout << value << std::endl;
    }

    // std::vector<int>::iterator
    // std::vector<int>::reverse_iterator
    // std::vector<int>::const_iterator
    // std::vector<int>::const_reverse_iterator

    for (std::vector<int>::iterator it = values.begin(); it != values.end(); it++) {
        std::cout << *it << std::endl;
    }

    using AgeMap = std::unordered_map<std::string, int>;
    AgeMap map{{"Vitaly", 35}, {"Maria", 33}};

    for (auto pair : map) {
        std::cout << pair.first << std::endl;
        std::cout << pair.second << std::endl;
    }

    for (auto [first, second] : map) {
        std::cout << first << std::endl;
        std::cout << second << std::endl;
    }

    for (AgeMap::const_iterator it = map.begin(); it != map.end(); it++) {
        auto& key = it->first;
        auto& value = it->second;
        std::cout << key << std::endl;
        std::cout << value << std::endl;
    }
}
