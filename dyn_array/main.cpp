#include <cstdlib>
#include <iostream>
#include <vector>

template <class T>
class Array {
public:
    Array() = default;

    ~Array()
    {
        std::free(_storage);
    }

    void push(T value)
    {
        if (_length >= _capacity) {
            if (_capacity > 0) {
                _capacity *= 2;
                if (std::realloc(_storage, _capacity) == nullptr) {
                    std::cout << "Array - Couldn't reallocate memory." << std::endl;
                    std::exit(1);
                }
            } else {
                _storage = static_cast<T*>(std::malloc(sizeof(T)));
                if (_storage == nullptr) {
                    std::cout << "Array - Couldn't allocated memory." << std::endl;
                    std::exit(1);
                }
                _capacity = 1;
            }
        }
        _storage[_length] = value;
        _length += 1;
    }

    T pop();

    T& first()
    {
        if (length() > 0) {
            return _storage[0];
        }
    }

    T& last()
    {
        if (length() > 0) {
            return _storage[_length - 1];
        }
    }

    bool compact();

    T operator[](size_t index)
    {
        if (index < 0 || index >= length()) {
            std::cout << "Array - Index out of bounds." << std::endl;
            std::exit(1);
        }
        return _storage[index];
    }

    inline size_t length() { return _length; }

    inline size_t capacity() { return _capacity; }

private:
    T* _storage;
    size_t _length { 0 };
    size_t _capacity { 0 };
};

int main(int, char**)
{
    Array<int> array;
    array.push(5);
    array.push(7);
    array.push(9);
    auto r = array[0];
    std::cout << "Result: " << array[0] << std::endl;
    std::cout << "Result: " << array[1] << std::endl;
    std::cout << "Length: " << array.length() << std::endl;
    std::cout << "Capacity: " << array.capacity() << std::endl;
    std::cout << "First: " << array.first() << std::endl;
    std::cout << "Last: " << array.last() << std::endl;

    std::vector<int> hey { 5 };
}
