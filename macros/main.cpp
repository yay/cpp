#include <iostream>

#define WAIT std::cin.get();  // BAD
#define OPEN_CURLY {          // OMG, WHY?

#define PR_DEBUG              // GOOD

#ifdef PR_DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x) // replace LOG(x) with nothing
#endif

// Multiline macro. Backslash escapes the new line character.
#define FUN void fun() {\
    std::cout << ":)" << std::endl;\
}

FUN

int main() OPEN_CURLY
    std::cout << "Hello\n";
    LOG("Vitaly");
    fun();
    WAIT
}
