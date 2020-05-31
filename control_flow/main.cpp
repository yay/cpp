#include <iostream>
#include <vector>
#include <bitset>

typedef std::bitset<8> BYTE;
int main(int, char**) {
    auto nums = std::vector<int>{1,2,3,5,7,11,256,257};

    std::cout << "Classic loop\n";
    for (int i = 0, ln = nums.size(); i < ln; i++) {
        std::cout << nums[i] << "\n";
    }

    std::cout << "Access by value, the type of n is int\n";
    for (auto n : nums) {
        // n = 5; // legal: n can be changed here
        std::cout << n << "\n";
    }

    std::cout << "Access by const reference\n";
    for (const int &n : nums) {
        // n = 5; // illegal: n is const
        std::cout << n << "\n";
    }

    std::cout << "Access by forwarding reference\n";
    for (auto &&n : nums) {
        // n = 5; // legal: n can be changed here
        std::cout << n << "\n";
    }

    const auto &constNums = nums;
    for (auto &&n : constNums) {
        // n = 5; // illegal: n is const
        std::cout << n << "\n";
    }

    for (int n : {0, 1, 2, 3, 4, 5}) {
        std::cout << n << "\n";
    }

    int a[] = {0, 1, 2, 3, 4, 5};
    for (int n : a) {
        std::cout << n << "\n";
    }

    for (auto n : nums) { // don't have to use the loop variable
        std::cout << "Hey\n";
    }

    for (BYTE n : nums) {
        std::cout << n << " size: " << sizeof(n) << "\n";
    }

}
