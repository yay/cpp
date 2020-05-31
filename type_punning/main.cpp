#include <iostream>

// Type punning - getting around the type system.
// While types are enforced by the compiler,
// you can directly access memory.

struct Empty {}; // instances will be (at least) 1 byte in memory
                 // because we need to address them somehow.

struct Entity
{
    int x, y;

    int *getPosition()
    {
        return &x;
    }
};

// https://www.reddit.com/r/cpp/comments/8pyq94/torvalds_on_aliasing/
template <typename T, typename S>
T pun_as(const S &val) {
    T t{};
    std::memcpy(&t, &val, std::min(sizeof(T), sizeof(S)));
    return t;
}

int main() {
    int a = 50;    // 4 bytes: 32 00 00 00
    // implicit conversion, same as `(double)a`
    double da = a; // 8 bytes: 00 00 00 00 00 00 49 40

    // type punning
    double dd = *(double *)&a; // get memory address of `a` (int pointer)
                               // and cast into a double pointer,
                               // then dereference it
    // This will go 4 bytes past our integer and copy them into our `dd`
    // variable. Even if we write to `dd` at this point, we won't be modifying
    // those 4 unrelated bytes, but we did read from them, which isn't good.
    double &dc = *(double *)&a; // will overwrite `a` memory and 4 bytes past it!

    std::cout << da << std::endl;
    std::cout << dd << std::endl;

    Empty empty;
    std::cout << "Size of `empty` is " << sizeof(empty) << std::endl;

    Entity e { 5, 8 };

    int *position = (int *)&e; // get the address of `e` and treat it as an array
    std::cout << position[0] << ", " << position[1] << std::endl;

    // C style cast
    int y = *(int*)((char*)&e + 4); // same as: int y = e.y;
    std::cout << "y: " << y << std::endl;

    // C++ way
    int yy = *reinterpret_cast<int*>(reinterpret_cast<char*>(&e) + 4);

    auto pos = e.getPosition();
    pos[0] = 7;
    pos[1] = 9;
    std::cout << "x: " << e.x << " y: " << e.y << std::endl;
}
