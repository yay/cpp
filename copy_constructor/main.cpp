#include <iostream>

struct Vector2 {
    float x;
    float y;
};

class String {
private:
    char *m_buffer;
    unsigned int m_size;
public:
    String(const char *string) {
        m_size = strlen(string);
        m_buffer = new char[m_size + 1];
        memcpy(m_buffer, string, m_size);
    }

    // Copy constructor:
    String(const String &other) : m_size(other.m_size) {
        std::cout << "Copied string.\n";
        m_buffer = new char[m_size];
        memcpy(m_buffer, other.m_buffer, m_size);
    }
    // Same as:
    // String(const String &other) : m_buffer(other.m_buffer), m_size(other.m_size) {}
    // Or:
    // String(const String &other) { memcpy(this, &other, sizeof(String)); }

    ~String() {
        delete[] m_buffer;
    }

    friend std::ostream &operator<<(std::ostream &stream, const String &string);
};

std::ostream &operator<<(std::ostream &stream, const String &string) {
    // since the << operator is a member of the String class, we can access
    // its private members in this function
    stream << string.m_buffer;
    return stream;
}

void printString(String string) { // needless copy
    std::cout << string << std::endl;
}

void printBetter(const String &string) {
    // Always pass your (big-ish) objects by const reference.
    // You can alway make a copy inside your method if need be:
    // String copy = string;
    std::cout << string << std::endl;
}

int main() {
    int a = 2;
    int b = a; // copy

    Vector2 v1 { 2, 3 };
    Vector2 v2 = v1; // copy

    Vector2 *v3 = new Vector2 { 3, 4 };
    Vector2 *v4 = v3; // v4 points to the same memory address as v3

    v4->x = 5;
    std::cout << v3->x << std::endl; // 5

    String string = "Vitaly";
    // If we don't have a copy constructor,
    String second = string; // we will copy the char* (but not the buffer itself) and the size,
    // so when we get to the end of the scope and destructors are called, we will try
    // to delete the m_buffer twice: "pointer being freed was not allocated"

    std::cout << string << std::endl;
    std::cout << second << std::endl;

    printString(string);
    printString(second);

    printBetter(string);
    printBetter(second);
}
