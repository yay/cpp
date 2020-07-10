#include <iostream>

class String {
public:
    String() = default;

    String(const char *string) {
        printf("String constructor call\n");
        m_Size = strlen(string);
        m_Data = new char[m_Size];
        memcpy(m_Data, string, m_Size);
    }

    String(const String &other) { // copy-constructor
        printf("String copy-constructor call\n");
        m_Size = other.m_Size;
        m_Data = new char[m_Size];
        memcpy(m_Data, other.m_Data, m_Size);
    }

    String(String &&other) noexcept { // move-constructor
        printf("String copy-constructor call\n");
        m_Size = other.m_Size;
        m_Data = other.m_Data;

        other.m_Size = 0;
        other.m_Data = nullptr;
    }

    ~String() {
        delete m_Data;
        printf("String destructor call\n");
    }

    void Print() {
        for (uint32_t i = 0; i < m_Size; i++)
            printf("%c", m_Data[i]);

        printf("\n");
    }

private:
    char *m_Data;
    uint32_t m_Size;
};

class Entity {
public:
    // copy-constructor will used unless the `name` is explicitly cast
    // to the `String&&` temporary
    Entity(const String &&name) : m_Name((String &&) name) {}

    void PrintName() {
        m_Name.Print();
    }

private:
    String m_Name;
};

int main() {
    {
        Entity entity(String("Vitaly"));
        entity.PrintName();
        // Prints:
        // > String constructor call      // constructing the temporary
        // > String copy-constructor call // constructing m_Name
        // > String destructor call       // destroying the moved temporary
        // > Vitaly
        // > String destructor call       // destroying m_Name
    }
    return 0;
}
