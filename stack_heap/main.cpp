#include <iostream>

struct Vector3
{
    float x, y, z;

    Vector3() : x(10), y(11), z(12) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

void printVector(const Vector3& vec) {
    std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
}

int main(int, char**) {
    int *hvalue = new int;     // heap int allocation (calls malloc which calls OS specific function)
    *hvalue = 7;

    // The program maintains a 'free list' which tracks which blocks of memory are free
    // and where they are.

    // 5 and [1, 2, 3, 4, 5] and Vector3 will be next to each other in memory
    // (there may be some bytes between them in debug mode)
    int value = 5;             // stack int
    int array[5];              // stack array
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;
    array[4] = 5;
    Vector3 stackVector;
    Vector3 stackVector2(3, 6, 9);

    printVector(stackVector);
    printVector(stackVector2);

    // Stack typicall grows backwards, from higher memory addresses to lower
    // memory addresses.
    // Stack allocation is very fast. Just one CPU instruction.
    // We just move the stack pointer and return its address.
    // For example, the 'value' and 'array[5]' allocation/initialization
    // is just the following assembly:
    // movl   $0x5, -0x8c(%rbp)  // int value = 5;
    // movl   $0x1, -0x20(%rbp)  // int array[5]; array[0] = 1;
    // movl   $0x2, -0x1c(%rbp)  // array[1] = 2;
    // movl   $0x3, -0x18(%rbp)  // array[2] = 3;
    // movl   $0x4, -0x14(%rbp)  // array[3] = 4;
    // movl   $0x5, -0x10(%rbp)  // array[4] = 5;

    // Once the scope in which allocation was made ends, the stack memory
    // allocated in that scope is freed. It doesn't have to be the end
    // of a function. Can be any kind of scope for/while loop, if statement,
    // inner scope, etc. For example:
    {
        double array[1];
        array[0] = 42;
    } // the array of doubles is freed here

    int *harray = new int[5];  // heap array
    harray[0] = 1;
    harray[1] = 2;
    harray[2] = 3;
    harray[3] = 4;
    harray[4] = 5;

    Vector3 *hvector = new Vector3();  // allocates vector on the heap, constructor is called
    Vector3 *hvector2 = (Vector3*)malloc(sizeof(Vector3)); // same, but constructor is not called

    Vector3 *hvectors = new Vector3[10]; // the block of memory large enough to accommodate
                                         // 10 vectors is contiguous, just like it would be
                                         // if we allocated on the stack

    // 'new' not only allocates memory, it also calls a constructor.
    // 'new' is just an operator, just like '+' or '=' and can be overloaded.

    int *b = new int[50];
    Vector3 *vec = new(b) Vector3(); // allocate at a specific memory address, in this case 'b'
    // delete vec; // can't delete both, because we'll get 'pointer being freed was not allocated'
    delete []b;

    delete hvalue;
    delete []harray;
    delete hvector; // calls the 'free' function
    free(hvector2);
    delete []hvectors;
}

// Use the following commands to create Xcode project:
//
//     mkdir xcode
//     cd xcode
//     cmake -G Xcode ..
