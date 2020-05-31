#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>

// https://mbevin.wordpress.com/2012/11/16/uniform-initialization/

struct Point
{
    int x, y;
};

void takePoint(Point p)
{
    std::cout << "Point taken: (" << p.x << ", " << p.y << ")" << std::endl;
}

int main(int, char**) {
    // Initializer lists
    int arr[] { 1, 2, 3, 4, 5 };
    std::vector<int> v { 1, 2, 3, 4, 5 };
    std::set<int> s { 1, 2, 3, 4, 5 };
    std::map<int, std::string> m { {0, "zero"}, {1, "one"}, {2, "two"} };

    // Uniform initialization
    int i {3};
    int j {}; // initialize to default: 0
    std::string str {"hello"};

    Point p {7, 8};
    takePoint(p);
    takePoint({3, 5});
}
