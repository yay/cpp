#include <cstdlib>
#include <iostream>
#include <vector>
#include <optional>

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

    std::optional<T> pop();

    std::optional<T> first()
    {
        if (length() > 0) {
            return _storage[0];
        }
        return {};
    }

    std::optional<T> last()
    {
        if (length() > 0) {
            return _storage[_length - 1];
        }
        return {};
    }

    bool compact();

    std::optional<T> operator[](size_t index)
    {
        if (index < 0 || index >= length()) {
            return {};
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
    std::cout << "array[0]: " << array[0].value() << std::endl;
    std::cout << "array[1]: " << array[1].value() << std::endl;

    auto first = array.first();
    if (first) {
        std::cout << "First: " << first.value() << std::endl;
    }
    auto last = array.last();
    if (last) {
        std::cout << "Last: " << last.value() << std::endl;
    }

    std::cout << "Length: " << array.length() << std::endl;
    std::cout << "Capacity: " << array.capacity() << std::endl;

    std::vector<int> hey { 5 };
}
