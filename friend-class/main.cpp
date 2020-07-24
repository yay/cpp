#include <iostream>

class Base {
public:
    Base() {}

    Base(int a) : x{a} {}

    friend void set(Base &, int x);
    friend int get(Base &);
    friend class GetSet; // this will compile even if `GetSet` is not defined
private:
    int x;
};

class GetSet {
public:
    void set(Base &obj, int x) {
        obj.x = x;
    }
    int get(Base &obj) {
        return obj.x;
    }
};

void set(Base &obj, int x) {
    obj.x = x; // `set` has access to the private field `x`
}

int get(Base &obj) {
    return obj.x; // `get` has access to the private field `x`
}

int main() {
    Base obj(10);
    set(obj, 20);
    std::cout << get(obj) << std::endl;

    GetSet gs;
    gs.set(obj, 7);
    std::cout << gs.get(obj) << std::endl;

    return 0;
}
