#include <cmath>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

struct Foo {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Foo() {}

    Foo(double x, double y, double z) : x(x), y(y), z(z) {
        std::cout << "Constructed Foo" << std::endl;
    }

    Foo(const Foo& other) : x(other.x), y(other.y), z(other.z) {
        std::cout << "Copy-constructed Foo" << std::endl;
    }

    Foo(const Foo&& other) : x(other.x), y(other.y), z(other.z) {
        std::cout << "Move-constructed Foo" << std::endl;
    }

    Foo& operator=(Foo&& other) {
        std::cout << "Moved Foo" << std::endl;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    ~Foo() {
        std::cout << "Destroyed Foo" << std::endl;
    }
};

// https://www.boost.org/doc/libs/1_65_0/libs/optional/doc/html/boost_optional/tutorial/performance_considerations.html
// https://www.bfilipek.com/2018/05/using-optional.html
template <class T>
class Array {
public:
    Array() = default;

    ~Array()
    {
        std::free(_storage);
    }

    bool push(T& value)
    {
        if (_size >= _capacity) {
            if (!grow_capacity()) {
                return false;
            }
        }
        _storage[_size] = value;
        _size += 1;
        return true;
    }

    bool push(T&& value)
    {
        if (_size >= _capacity) {
            if (!grow_capacity()) {
                return false;
            }
        }
        _storage[_size] = std::move(value);
        _size += 1;
        return true;
    }

    std::optional<T> pop()
    {
        auto last = this->last();
        if (_size > 0) {
            _size -= 1;
        }
        return std::move(last);
    }

    std::optional<T> first()
    {
        if (size() > 0) {
            return _storage[0];
        }
        return {};
    }

    std::optional<T> last()
    {
        if (size() > 0) {
            return _storage[_size - 1];
        }
        return {};
    }

    bool compact()
    {
        auto new_capacity = std::pow(2, std::ceil(std::log2(_size)));
        if (std::realloc(_storage, new_capacity * sizeof(T)) == nullptr) {
            return false;
        }
        _capacity = new_capacity;
        return true;
    }

    std::optional<T> operator[](size_t index)
    {
        if (index < 0 || index >= size()) {
            return {};
        }
        return _storage[index];
    }

    inline size_t size() { return _size; }

    inline size_t capacity() { return _capacity; } // zero or a number that is a power of two

private:
    T* _storage;
    size_t _size { 0 };
    size_t _capacity { 0 };

    bool grow_capacity()
    {
        if (_capacity > 0) {
            auto new_capacity = _capacity * 2;
            if (std::realloc(_storage, new_capacity * sizeof(T)) == nullptr) {
                return false;
            }
            std::cout << "Reallocated dynamic array storage to hold up to " << new_capacity
                      << " elements" << std::endl;
            _capacity *= new_capacity;
        } else {
            auto new_capacity = 1;
            _storage = static_cast<T*>(std::malloc(new_capacity * sizeof(T)));
            if (_storage == nullptr) {
                return false;
            }
            std::cout << "Allocated dynamic array storage to hold 1 element" << std::endl;
            _capacity = new_capacity;
        }
        return true;
    }
};

void pop_value(Array<int>& array)
{
    if (auto value = array.pop()) {
        std::cout << "Popped value: " << *value << std::endl;
    } else {
        std::cout << "Nothing to pop" << std::endl;
    }
}

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
    if (auto last = array.last()) {
        std::cout << "Last: " << *last << std::endl;
    }

    std::cout << "Length: " << array.size() << std::endl;
    std::cout << "Capacity: " << array.capacity() << std::endl;

    pop_value(array);
    pop_value(array);
    pop_value(array);
    pop_value(array);

    Array<Foo> fooray;
    fooray.push(Foo { 1.0, 2.0, 3.0 });
    if (auto first_foo = fooray.pop()) {
        std::cout << "First foo: " << (*first_foo).z << std::endl;
    }
}
