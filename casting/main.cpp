#include <print>

class Base
{
public:
    Base() {}
    virtual ~Base() {}
};

class Derived : public Base
{
public:
    Derived() {}
    ~Derived() {}
};

class AnotherClass : public Base
{
public:
    AnotherClass() {}
    ~AnotherClass() {}
};

int main(int, char **)
{
    {
        double a1 = 5.25;
        double a2 = (int)a1 + 5.3; // C style cast
        std::println("{}", a2);
    }
    {
        double a1 = 5.25;
        double a2 = static_cast<int>(a1) + 5.3; // C++ style cast
        std::println("{}", a2);
    }
    // C++ style casts: static cast, reinterpret, dynamic, constant.
    // Don't do anything that C style casts can't do.
    // Introduces compiler checks.
    // Makes casts more searchable in code.

    {
        Derived *derived = new Derived();
        Base *base = derived;

        AnotherClass *ac = dynamic_cast<AnotherClass *>(base); // does run-time checking via RTTI
        if (ac == nullptr)
        {
            std::println("Conversion failed");
        }
    }
}
