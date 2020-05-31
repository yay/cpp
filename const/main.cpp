#include <iostream>

// const - a promise that you won't modify things

class Entity
{
private:
    int m_X, m_Y;
    int *m_W, *m_H; // without the * before the m_H,
                    // only the m_W is going to be int*
                    // and the m_H is going to be int
    mutable int var;
public:
    int getX() const // method can't modify the Entity
    {
        // m_X = 2;  // illegal
        // m_Y = 5;  // illegal
        return m_X;
    }
    int getY()
    {
        return m_Y;
    }
    void setX(int x)
    {
        m_X = x;
    }

    // What this means is:
    // 1) the contents of the pointer cannot be modified
    // 2) the pointer itself cannot be modified
    // 3) the Entity can't be modified by the method
    const int *const getW() const
    {
        return m_W;
    }

    // Sometimes you just need a const method to modify a member variable.
    // For that we need to mark that member variable as mutable.
    const std::tuple<int, int> getXY() const
    {
        var = 2;
        return {m_X, m_Y};
    }
};

void printEntity(const Entity &e)  // pass by const reference, so that we don't copy the Entity
{
    std::cout << e.getX() << std::endl;
    // std::cout << e.getY() << std::endl; // error: e is const Entity, but getY is not marked const

    // To be able to use the getY method in both instances (const Entity &e and Entity &e),
    // we need to declare the getY twice inside the Entity definition, once with const and once without:
    //
    //     int getY() const
    //     {
    //         return m_Y;
    //     }
    //     int getY()
    //     {
    //         return m_Y;
    //     }
}

void printEntityPtr(const Entity *e)  // pass by const reference, so that we don't copy the Entity
{
    e = nullptr;
    std::cout << e->getX() << std::endl;
}

int main(int, char**) {
    const int MAX_AGE = 77;

    int *a = new int;    // create an integer on the heap to get a pointer
    *a = 2;              // can change both the value at the memory address
    a = (int*)&MAX_AGE;  // and the address (pointer) itself

    // If we try to
    // delete a;
    // at this point, we'll get "pointer being freed was not allocated"
    // because the pointer is now pointing at the stack allocated data

    const int *b = new int;
    int const *c = new int; // this is the same as above (the above is more conventional)
    // *b = 7;              // can't change data at the memory address
    b = (int*)&MAX_AGE;     // but still can change the pointer itself


    int *const d = new int;
    *d = 5;                 // can change the data at the memory address
    // d = (int*)&MAX_AGE;  // but can't change the pointer itself

    const int *const e = new int;
    // *e = 9;              // can't change neither the data at the address
    // e = (int*)&MAX_AGE;  // nor the address itself

    std::cout << *a << "\n";
    std::cout << *b << "\n";

    Entity entity;
}
