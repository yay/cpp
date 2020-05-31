#include <iostream>
#include <memory>

void rawPointers() {
    void* ptr = 0; // void means that the pointer is typeless
    // 0 is not a valid memory address (not a valid pointer)
    ptr = NULL;    // C define
    ptr = nullptr; // keyword introduced in C++11

    int var = 8;
    void* varPtr = &var; // & takes the memory address of a variable
    int* intPtr = &var;
    double* dblPtr = (double*)&var;

    // all 3 are going to print the same address:
    std::cout << varPtr << "\n";
    std::cout << intPtr << "\n";
    std::cout << dblPtr << "\n";

    // * is dereferencing (get access to the data from pointer)
    // *varPtr = 10; // the compiler doesn't know how many bytes to write:
                     // is 10 a byte, short, int, long?
    *intPtr = 10;

    std::cout << var << "\n"; // output the value of the original variable: 10

    // create a variable on the heap
    char* buffer = new char[8]; // allocated 8 bytes of memory and return a pointer
                                // to the beginning of that block
    memset(buffer, 0, 8);       // fill a block of memory (pointer, value, number of bytes)

    char** charPtr = &buffer;

    delete[] buffer;            // deallocated the buffer
}

class Entity {
public:
    Entity() {
        Entity::id++;
        std::cout << "Created Entity #" << id << std::endl;
    }

    ~Entity() {
        std::cout << "Destroyed Entity #" << id << std::endl;
    }

    void print() {
        std::cout << "Hello from Entity #" << id << std::endl;
    }
private:
    static int id;
};
int Entity::id = 0;

void smartPointers() {
    {
        // Unique pointers can't be copied (for example, passed into a function).
        // If they could, when one pointer was freed, the other would become invalid.
        std::unique_ptr<Entity> entity(new Entity);
        entity->print();
    }

    {
        // The preferred way. If the constructor throws an exception,
        // you won't end up with a dangling reference.
        std::unique_ptr<Entity> entity = std::make_unique<Entity>();
        // auto anotherEntity = entity; // error, copy-constructor and copy-assigment operators are deleted:
        // unique_ptr(const unique_ptr&) = delete;
        // unique_ptr& operator=(const unique_ptr&) = delete;
    }

    std::shared_ptr<Entity> anotherSharedEntity;
    std::weak_ptr<Entity> weakEntity;
    {
        std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
        anotherSharedEntity = sharedEntity; // increases the ref count
        weakEntity = sharedEntity; // doesn't increase the ref count, so doesn't keep the Entity alive
    }
    std::cout << "Shared entity is not destroyed yet." << std::endl;
}

int main(int, char**)
{
    rawPointers();
    smartPointers();

    return 0;
}