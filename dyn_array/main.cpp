#include <cstdlib>
#include <cmath>
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

    bool push(T value)
    {
        if (_length >= _capacity) {
            if (_capacity > 0) {
                auto new_capacity = _capacity * 2;
                if (std::realloc(_storage, new_capacity * sizeof(T)) == nullptr) {
                    return false;
                }
                _capacity *= new_capacity;
            } else {
                auto new_capacity = 1;
                _storage = static_cast<T*>(std::malloc(new_capacity * sizeof(T)));
                if (_storage == nullptr) {
                    return false;
                }
                _capacity = new_capacity;
            }
        }
        _storage[_length] = value;
        _length += 1;
        return true;
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

    bool compact() {
        auto new_capacity = std::pow(2, std::ceil(std::log2(_length)));
        if (std::realloc(_storage, new_capacity * sizeof(T)) == nullptr) {
            return false;
        }
        _capacity = new_capacity;
        return true;
    }

    std::optional<T> operator[](size_t index)
    {
        if (index < 0 || index >= length()) {
            return {};
        }
        return _storage[index];
    }

    inline size_t length() { return _length; }

    inline size_t capacity() { return _capacity; } // zero or a number that is a power of two

private:
    T* _storage;
    size_t _length { 0 };
    size_t _capacity { 0 };
};

int main(int, char**)
{
    Array<int> array;
    std::cout << "Last element is over " << array.last().value_or(9000) << std::endl;
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
        std::cout << "Last: " << *last << std::endl;
    }

    std::cout << "Length: " << array.length() << std::endl;
    std::cout << "Capacity: " << array.capacity() << std::endl;

    std::vector<int> hey { 5 };
}
