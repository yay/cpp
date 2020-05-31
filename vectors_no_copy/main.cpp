#include <iostream>
#include <vector>

struct Vertex {
    float x, y, z;

    Vertex(): x(0), y(0), z(0) {} // default constructor

    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

    // copy constructor
    Vertex(const Vertex &other) : x(other.x), y(other.y), z(other.z) {
        std::cout << "Copied Vertex!" << std::endl;
    }

    void print(const std::string &msg) {
        std::cout << msg << " (x: " << x << ", y: " << y << ", z: " << z << ")" << std::endl;
    }
};

void separator() {
    std::cout << "---------------" << std::endl;
}

int main() {
    std::vector<Vertex> vec1;
    vec1.push_back(Vertex(1, 2, 3)); // creating one copy
    separator();
    // Because...
    // - the Vertex is created on the stack frame of the `main` function
    // - then copied into the memory the vector has allocated

    std::vector<Vertex> vec2 { {4, 5, 6}, {7, 8, 9} }; // creating two copies
    separator();

    std::vector<Vertex> vec3;
    // have 6 copies total below
    vec3.push_back(Vertex(-1, -2, -3)); // copy {-1, -2, -3} into the vector allocated memory
    vec3.push_back(Vertex(-4, -5, -6)); // resize the vector, copy {-4, -5, -6} into the vector, then {-1, -2, -3} again
    vec3.push_back(Vertex(-7, -8, -9)); // resize the vector, copy {-7, -8, -9}, then {-4, -5, -6}, then {-1, -2, -3}
    separator();

    std::vector<Vertex> vec4;
    vec4.reserve(3); // reserve space for 3 vertices outright
    // 3 copies below
    vec4.push_back(Vertex(-1, -2, -3));
    vec4.push_back(Vertex(-4, -5, -6));
    vec4.push_back(Vertex(-7, -8, -9));
    separator();

    // No copies anywhere!
    std::vector<Vertex> reserveVec;
    reserveVec.reserve(3); // reserves memory only
    // may print different values each time - whatever is in the initialized memory at the time
    reserveVec[0].print("Reserve only");
    reserveVec.emplace_back(1, 2, 3); // construct a Vertex inside the vector memory
    reserveVec.emplace_back(3, 4, 5);
    reserveVec.emplace_back(6, 7, 8);
    separator();

    std::vector<Vertex> defaultVec(3); // create a vector with 3 default initialized vertices
    // Default initialization would fail with:
    // "candidate constructor not viable: requires n arguments, but 0 were provided"
    // if we didn't have a default constructor (one that takes no arguments) defined inside
    // the Vertex struct.

    defaultVec[0].print("Default init");

	return 0;
}
