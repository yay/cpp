#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

std::tuple<int, double, char> foo_tuple() {
    return {1, -1.0, 'A'};
}

std::tuple<std::string, int> getPerson() {
    return {"Vitaly Kravchenko", 34};
}

void structuredBindings() {
    auto [name, age] = getPerson(); // can also use "auto&" to refer rather than copy
    std::cout << name << " is " << age << std::endl;

    int x = 5;
    int y = 7;
    std::tuple<int&, int&> tie = std::tie(x, y);

    struct S {
        int n;
        std::string s;
        float d;
        bool operator<(const S& rhs) const {
            // compares n to rhs.n,
            // then s to rhs.s,
            // then d to rhs.d
            return std::tie(n, s, d) < std::tie(rhs.n, rhs.s, rhs.d);
        }
    };
}

int main() {
    std::list<int> list{1, 2, 3, 4, 5, 6};
    auto pred = [](int a, int b) { return b % 2 ? a : a + b; };
    auto sumEven = accumulate(list.begin(), list.end(), 0, pred);
    assert(sumEven == 12);

    int (*fnPtr)(int, int) = pred;
    int val = fnPtr(7, 3);

    auto allArePositive = all_of(list.begin(), list.end(), [](int a) { return a > 0; });
    std::cout << "All are positive: " << allArePositive << std::endl;

    std::vector<int> vec{1, 2, -3, 4, -5, 6};
    auto fn = [](int a) -> bool {
        return a > 0;
    };
    for (long i = 0, ln = vec.size(); i < ln; i++) {
        if (!fn(vec[i])) {
            std::cout << vec[i] << " with index " << i << " is < 0" << std::endl;
        }
    }

    std::cout << "Range for: ";
    for (auto e : vec) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    std::function<int(int)> takeInt = [](int x) -> int { return x; };
    std::cout << "take " << takeInt(5) << std::endl;

    for (const auto& i : vec) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    auto tuple = foo_tuple();
    std::cout << "(" << std::get<0>(tuple) << ", " << std::get<1>(tuple) << ", " << std::get<2>(tuple) << ")" << std::endl;

    //    double number;
    //    cout << "Enter a number: ";
    //    cin >> number;
    //    cout << "You entered " << number << endl;

    structuredBindings();

    return 0;
}