#include <string>
#include <print>

class Entity
{
public:
    void print() const
    {
        std::println("Hello");
    }
};

struct Vector3
{
    float x, y, z;
};

// Will automatically delete the Entity when it goes out of scope.
class ScopePtr
{
private:
    Entity *m_obj;

public:
    ScopePtr(Entity *entity) : m_obj(entity) {}
    ~ScopePtr() { delete m_obj; }

    Entity *operator->() { return m_obj; }
    const Entity *operator->() const { return m_obj; }
};

void overloading()
{
    ScopePtr entity = new Entity();
    entity->print(); // without overloading the `->` operator
                     // this wouldn't be possible

    // What we do below is:
    // - take 0
    // - cast it into a Vector3 pointer
    // - dereference it
    // - get the value of x
    // - get the address of x
    float *x = &((Vector3 *)0)->x;
    std::println("{}", uintptr_t(x));

    auto offsetX = &((Vector3 *)nullptr)->x;
    auto offsetY = &((Vector3 *)nullptr)->y;
    auto offsetZ = &((Vector3 *)nullptr)->z;
    std::println("{}", uintptr_t(offsetX)); // 0
    std::println("{}", uintptr_t(offsetY)); // 4
    std::println("{}", uintptr_t(offsetZ)); // 8

    // In other words, we can use `->` to get the offset of the data in memory.
}

int main()
{
    Entity e;
    e.print();

    Entity *ptr = &e;
    // ptr.print(); // pointer doesn't have any methods,
    // have to dereference it first
    (*ptr).print();

    // or
    ptr->print();

    // or
    Entity &entity = *ptr;
    entity.print();

    std::println("----------");
    overloading();

    return 0;
}
