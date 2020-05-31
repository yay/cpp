#include <iostream>

template<class T>
void println(T value) {
    std::cout << value << std::endl;
}

template<class T, int SIZE>
class Array {
private:
    T m_array[SIZE] {}; // use {} for default initialization
public:
    Array() {}
    int size() const { return SIZE; }
    T operator[](const int index) { return m_array[index]; }
};

template<class T> class X {
    X<T> *p; // OK
    X<T*> a; // recursive template instantiation exceeded maximum depth of 1024
};

int main() {
    println(5);
    println<int>(5);
    println("hello");
    println(5.5f);

    Array<int, 5> array;
    println(array.size());

    for (int i = 0, ln = array.size(); i < ln; i++) {
        println(array[i]);
    }

    // X<int> x; // uncomment to get an error
}
