#include <iostream>
#include <array>
#include <vector>

class Entity {
public:
    static const int exampleSize = 5;
    int example[exampleSize];

    Entity() {
        for (int i = 0; i < exampleSize; i++)
            example[i] = 2;
    }
};

class EntityHeapValue {
public:
    int *example = new int[5];

    EntityHeapValue() {
        for (int i = 0; i < 5; i++)
            example[i] = 2;
    }

    ~EntityHeapValue() {
        delete[] example;
    }
};

struct Vertex {
    float x, y, z;
};

std::ostream &operator<<(std::ostream &stream, const Vertex &vertex) {
    stream << vertex.x << ", " << vertex.y << ", " << vertex.z << std::endl;
    return stream;
}

void printHeader(const char *msg) {
    printf("----- %s -----\n", msg);
}

void vectors() {
    printHeader("C++ Vectors");
    // When the initial capacity of a vector is exceeded, a new larger block of memory
    // is allocated, the elements are copied there and the old block of memory is deleted.

    std::vector<Vertex> vertices;         // contiguous memory, but resizing is slow, copies actual vertices
    std::vector<Vertex *> vertexPointers; // can be fragmented, but resizes much faster, copies pointers

    vertices.push_back({1, 2, 3});
    vertices.push_back({4, 5, 6});
    vertices.push_back({7, 8, 9});
    vertices.push_back({10, 11, 12});
    vertices.push_back({13, 14, 15});

    // `erase` takes an interator, not an index
    vertices.erase(vertices.begin() + 1); // remove second element
    vertices.erase(vertices.begin() + 1, vertices.begin() + 3); // remove second through third elements

    for (int i = 0; i < vertices.size(); i++) {
        std::cout << vertices[i];
    }

    for (const auto &v : vertices) {
        std::cout << v;
    }

    vertices.clear();

    // --- Initialization gotchas ---
    std::vector<int> v1(100, 1);  // A vector containing 100 items: All 1s.
    std::vector<int> v2{100, 1};  // A vector containing 2 items: 100 and 1.
    std::cout << "v1 size: " << v1.size() << std::endl;
    std::cout << "v2 size: " << v2.size() << std::endl;
}

void c_arrays() {
    printHeader("C Arrays");

    int plain[3] = {6, 7, 8}; // create array on the stack
    // No (standard) way to get the number of elements in an plain C array,
    // but we can do this for stack arrays only:
    int plainCount = sizeof(plain) / sizeof(int);
    std::cout << "'plain' array countains " << plainCount << " elements\n";
    // since plain is just an int pointer we can do
    int *ptr = plain;

    // prints the same value: 7
    std::cout << plain[2] << std::endl;
    std::cout << *(ptr + 2) << std::endl; // +2 will add the number of bytes that depend on the type

    // prints the same memory address
    std::cout << &plain[2] << std::endl;
    std::cout << (ptr + 2) << std::endl;

    // To add in bytes, not in ints we can cast the pointer to a char pointer,
    // add the number of bytes we want, in this case sizeof(int) * 2 bytes,
    // and then cast the pointer back to int pointer.
    *(int *) ((char *) ptr + sizeof(int) * 2) = 9;

    std::cout << plain[2] << std::endl; // prints 9

    int a[2][3] = {{1, 2, 3},  // can be viewed as a 2x3 matrix
                   {4, 5, 6}}; // with row-major layout

    // More array info: https://en.cppreference.com/w/cpp/language/array

    int b[] = {0, 1, 2, 3, 4};
    for (auto x : b) { // can iterate `b`, so compiler knows its size, but we can't get it
        std::cout << x << " ";
    }
    std::cout << std::endl;

    int *heapArray = new int[5]; // create array on the heap (mainly to deal with lifetimes)
    for (int i = 0; i < 5; i++)
        heapArray[i] = 7 + i;
    std::cout << *heapArray << std::endl; // *heapArray - value at the address, i.e. first element: 7
    int heapCount = sizeof(heapArray) / sizeof(int); // WRONG: this will always return 2 on x64 architecture:
    // size of pointer / size of int = 8 / 4 = 2
    // It's not possible to get the number of elements of a heap array in a portable manner.
    std::cout << "'heapArray' countains " << heapCount << " elements\n";
    delete[] heapArray;

    // inspect both entities in memory view
    Entity e;            // entity's memory starts with the data of the array
    EntityHeapValue ehv; // entity's memory starts with the address of array
    // which if reversed (because of endianness) and followed will
    // show the data of the array
}

void cpp_arrays() {
    printHeader("C++ Arrays");

    std::array<int, 3> array = {{1, 2, 3}};
}

void gotcha() {
    std::vector<std::string> v;
    v.push_back("Hello, ");
    std::string &x = v[0];
    v.push_back(" world");
    std::cout << "x: " << x; // ERROR (or doesn't print "Hello, ")
    // The memory x is pointing to has been destroyed when we pushed
    // " world", because the vector only had a chunk of memory to hold
    // a single string, it had to allocate another chunk to hold two
    // strings and copied the first one, then destroyed the original chunk
    // which x still points to.
}

int main() {
    c_arrays();
    cpp_arrays();
    vectors();
    gotcha();

	return 0;
}
