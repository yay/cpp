#include <iostream>

// A static local variable has the lifetime of the entire program,
// but its scope is limited to the scope it is in.

void fun() {
    static int i = 0; // it's as if `i` was declared outside a function:
                      // int i = 0;
                      // but you can't access `i` outside of `fun`
    std::cout << i++ << std::endl;
}

class WeirdSingleton {
    static WeirdSingleton *instance;
    WeirdSingleton(); // prevent instantiation
public:
    static WeirdSingleton &get() { return *instance; }

    void hello() { std::cout << "Weird ..." << std::endl; }
};

WeirdSingleton *WeirdSingleton::instance = nullptr;

class Singleton {
    // Does not prevent instantiation!
    // TODO: Linking error when using a private constructor.
public:
    static Singleton &instance() {
        static Singleton singleton;
        return singleton;
    }

    void hello() { std::cout << "Hello" << std::endl; }
};

// Notes on singletons:
// - global state
// - hide dependencies
// - potential safety or performance issues with multiple threads
// - harder to unit test in isolation

// Singletons are good for resources that have a single instance
// that needs to be managed (for example, a log file).

struct Entity {
    int x, y;

    void print() {
        std::cout << x << ", " << y << std::endl;
    }

    // static void hello() {
    //     x = 5; // can't access instance variables
    // }
};

// Essentially how the non-static print method of the Entity works:
static void print(Entity e) {
    std::cout << e.x << ", " << e.y << std::endl;
}

struct Point {
    static int x, y;

    void print() {
        std::cout << x << ", " << y << std::endl;
    }

    static void staticPrint() {
        std::cout << x << ", " << y << std::endl;
    }
};

// To prevent undefined symbols Point::x, Point::y error, we define them like so:
int Point::x; // don't have to set them to anything here, but can if we want to
int Point::y;

void testFun()
{
    fun(); // 0
    fun(); // 1
    fun(); // 2
}

void testEntity() {
    Entity e1;
    e1.x = 2;
    e1.y = 3;

    Entity e2 = { 5, 6 };

    // Point p1 = { 3, 4 };  // initializer won't work because the class has no member variables

    Point p2;
    p2.x = 7;
    p2.y = 8;

    Point p3;
    p3.x = 3;
    p3.y = 6;

    p2.print(); // 3, 6
    p3.print(); // 3, 6

    Point::x = 15;
    Point::y = 30;

    p3.print();
    Point::staticPrint();
}

int main(int, char**) {
    testFun();

    testEntity();

    WeirdSingleton::get().hello();
    Singleton::instance().hello();
}
