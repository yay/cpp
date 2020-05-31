#include <iostream>
#include <string>
#include <unordered_map>

// https://bitbashing.io/std-visit.html

// Remember to:
// - update the tag whenever assigning a new value
// - only retrieve the correct type from the union
// - call constructors and destructors at appropriate times
//   for all non-trivial types (string is the only one here)
struct Setting {
    union {
        std::string str;
        int num;
        bool b;
    };
    enum Type { Str, Int, Bool };
    Type tag;
};

void basic() {
    struct Uni
    {
        // Two different ways to address the same memory.
        // A way to do type punning.
        union {
            float a;
            int b;
        };
    };

    Uni u;
    u.a = 2.0f;

    printf("%f\n", u.a);
    printf("%i\n", u.b);
}

struct Vector2 {
    float x, y;
};

struct Vector4 {
    union {
        struct {
            float x, y, z, w;
        };
        struct {
            Vector2 a, b;
        };
    };
};

void printVector2(const Vector2 &vector) {
    printf("%.2f, %.2f\n", vector.x, vector.y);
}

void vec() {
    Vector4 vector { 1.0f, 2.0f, 3.0f, 4.0f };
    printVector2(vector.a);
    printVector2(vector.b);

    vector.z = 100.0f;
    printVector2(vector.b);
}

int main(int, char**) {
    using Setting = std::unordered_map<std::string, Setting>;

    basic();
    vec();
}
