#include <cmath>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>
#include <assert.h>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

static int FooId = 0;
struct Foo {
    int id = ++FooId;
    double x{0};
    double y{0};
    double z{0};

    Foo() {}

    Foo(double x, double y, double z)
        : x(x), y(y), z(z) {
        std::cout << "Constructed Foo-" << id << std::endl;
    }

    Foo(const Foo& other)
        : x(other.x), y(other.y), z(other.z) {
        // Don't copy the id on clone, let it increment.
        std::cout << "Copy-constructed Foo-" << id << " from Foo-" << other.id << std::endl;
    }

    Foo(Foo&& other) noexcept
        : id(other.id), x(other.x), y(other.y), z(other.z) {
        // Copy the id on move.
        std::cout << "Move-constructed Foo-" << id << std::endl;
    }

    Foo& operator=(const Foo& other) {
        std::cout << "Copy-assigned Foo-" << other.id << std::endl;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Foo& operator=(Foo&& other) {
        std::cout << "Move-assigned Foo-" << other.id << std::endl;
        // The left hand side fields here are all zero initialized.
        id = other.id;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    ~Foo() {
        std::cout << "Destroyed Foo-" << id << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& out, const Foo& foo) {
        out << "Foo(" << foo.x << ", " << foo.y << ", " << foo.z << ")";
        return out;
    }
};

// https://www.boost.org/doc/libs/1_65_0/libs/optional/doc/html/boost_optional/tutorial/performance_considerations.html
// https://www.bfilipek.com/2018/05/using-optional.html
template <class T>
class Array {
public:
    Array() = default;

    ~Array() {
        std::free(_storage);
    }

    bool push(T& value) {
        if (_size >= _capacity) {
            if (!grow_capacity()) {
                return false;
            }
        }
        _storage[_size] = value;
        _size += 1;
        return true;
    }

    bool push(T&& value) {
        if (_size >= _capacity) {
            if (!grow_capacity()) {
                return false;
            }
        }
        _storage[_size] = value;
        _size += 1;
        return true;
    }

    std::optional<T> pop() {
        if (size() > 0) {
            _size -= 1;
            return _storage[_size];
        }
        return {};
    }

    // first() and last() return optional just to show how they
    // could be used and why it's not ideal to use them here.
    std::optional<T> first() {
        if (size() > 0) {
            return _storage[0];
        }
        return {};
    }

    std::optional<T> last() {
        if (size() > 0) {
            return _storage[_size - 1];
        }
        return {};
    }

    bool compact() {
        auto new_capacity = std::pow(2, std::ceil(std::log2(_size)));
        if (std::realloc(_storage, new_capacity * sizeof(T)) == nullptr) {
            return false;
        }
        _capacity = new_capacity;
        return true;
    }

    const T& operator[](size_t index) const {
        assert(index < size() && index >= 0);
        return _storage[index];
    }

    inline size_t size() const { return _size; }

    inline size_t capacity() { return _capacity; } // zero or a number that is a power of two

    friend std::ostream& operator<<(std::ostream& out, const Array& array) {
        out << "Array(" << std::endl;
        auto ln = array.size();
        for (size_t i = 0; i < ln; i++) {
            std::cout << "\t" << array[i] << std::endl;
        }
        out << ")";
        return out;
    }

private:
    T* _storage{nullptr};
    size_t _size{0};
    size_t _capacity{0};

    bool grow_capacity() {
        if (_capacity > 0) {
            auto new_capacity = _capacity * 2;
            auto storage = std::realloc(_storage, new_capacity * sizeof(T));
            if (storage == nullptr) {
                return false;
            }
            _storage = static_cast<T*>(storage);
            std::cout << "Reallocated dynamic array storage to hold up to " << new_capacity
                      << " elements" << std::endl;
            _capacity *= new_capacity;
        } else {
            auto new_capacity = 1;
            auto storage = std::malloc(new_capacity * sizeof(T));
            if (storage == nullptr) {
                return false;
            }
            _storage = static_cast<T*>(storage);
            std::cout << "Allocated dynamic array storage to hold 1 element" << std::endl;
            _capacity = new_capacity;
        }
        return true;
    }
};

void pop_value(Array<int>& array) {
    if (auto value = array.pop()) {
        std::cout << "Popped value: " << *value << std::endl;
    } else {
        std::cout << "Nothing to pop" << std::endl;
    }
}

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 20);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(int, char**) {
    signal(SIGSEGV, handler); // print stack trace for segmentation faults

    // Array<Foo> fooray;
    // fooray.push(Foo{1.0, 2.0, 3.0});
    // if (auto first_foo = fooray.pop()) {
    //     std::cout << "First foo: " << (*first_foo).z << std::endl;
    // }
    // fooray.push(Foo{3.0, 5.0, 7.0});
    // fooray.push(Foo{3.0, 5.0, 7.0});
    // return 0;

    Array<int> array;
    std::cout << "Last element is over " << array.last().value_or(9000) << std::endl;
    array.push(5);
    array.push(7);
    array.push(9);
    std::cout << "array[0]: " << array[0] << std::endl;
    std::cout << "array[1]: " << array[1] << std::endl;

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

    Foo f1{7.0, 8.0, 9.0};
    Foo f2{f1};

    Array<Foo> fooray;
    fooray.push(Foo{1.0, 2.0, 3.0});
    if (auto first_foo = fooray.pop()) {
        std::cout << "First foo: " << (*first_foo).z << std::endl;
    }
    fooray.push(Foo{3.0, 5.0, 7.0});
    fooray.push(Foo{9.0, 6.0, 3.0});
    
    std::cout << fooray[0] << std::endl;
    std::cout << fooray << std::endl;
}
