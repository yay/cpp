#include <exception>
#include <stdexcept>
// Some other standard exceptions are declared here:
// #include <new>
// #include <typeinfo>
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <assert.h>

// Jonathan Blow's comments:

// - If you don't test your code that uses exceptions thoroughly
//   for every possible exception that can be thrown, you get an
//   illusion of error handling.

// - Even if you end up with correct code, they give you an extra
//   concern that you have to reason about everywhere in your code.
//   Linear code is hard enough to write, now you have to think
//   about who can throw what, when and what do we do when that
//   happens. Go read "Exceptional C++", if you think that's worth it.

// - Handling errors explicitly is good. Not only because it makes
//   your code much simpler, because it's self-documenting,
//   also it's higher performance. If you read "Exceptional C++"
//   and look at what they do to make their data structures exception
//   safe, it often has severe performance implications.
//   Because you have to do things such that you can perform an atomic
//   transaction that is guaranteed not to throw in order to move
//   from an old state to a new state. So you can't just mutate things.
//   You have to like store the new value and the old value and then
//   swap them really sly at the last minute. But that's slow because
//   you are likely allocating more memory to store things temporarily.

// - In reply to this comment: The "Exceptional C++" book was written
//   way before move semantics was added to C++, which makes things
//   much easier since move constructors tend to be noexcept.
//   Blow said: well, they tend to, but are they guaranteed to?
//   The move constructor is now an additional requirement on the
//   correctness of your data structure.

// - The only types of exceptions I deem acceptable are panics.

// My questions:
// - What about stack traces? How do you do this without exceptions?
//   Do panics give you those? Answer: I see it's possible to set the
//   RUST_BACKTRACE environment varible to 1 to get a backtrace in Rust.
// - What happens when an error is not handled? Does the program just
//   continue execution as if nothing happened?

// The issue with error codes is that if you really want to do things
// properly, you must propagate them manually all the way up the call
// stack, where it makes sense.

void throw_everything() {
    try {
        throw 5;
        std::cout << "You'll never see this" << std::endl;
    } catch (int error) {
        std::cout << "Caught: " << error << std::endl;
    }

    try {
        throw "Hello, my name is exception";
    } catch (const char* error) {
        std::cout << "Caught: " << error << std::endl;
    }

    try {
        throw true;
    } catch (bool error) {
        std::cout << "Caught: " << (error ? "true" : "false") << std::endl;
    }

    try {
        const int answer = 42;
        throw std::tie("Life is ", answer);
    } catch (std::tuple<char const(&)[9], int const&> error) {
        auto [one, two] = error;
        std::cout << "Caught: " << one << two << std::endl;
    }

    try {
        throw std::invalid_argument("69");   // exceptions are usually thrown by value
    } catch (std::invalid_argument& error) { // and caught by reference
        std::cout << "Caught: " << error.what() << std::endl;
    }

    class MyException : std::exception {
    public:
        const char* what() const noexcept {
            return "Blah";
        }
    };

    try {
        throw MyException();
    } catch (MyException& error) {
        std::cout << "Caught: " << error.what() << std::endl;
    }

    throw std::logic_error("something");
}

void catch_everything() {
    try {
        throw 5;
        throw "LOL";
        throw std::logic_error("Duh");
    } catch (...) {
        std::cout << "Caught something ¯\\_(ツ)_/¯" << std::endl;
    }
    // The above is rarely used. Usually something like this is done:
    auto heap_int = new int;
    try {
        try {
            throw "Bad stuff";
        } catch (...) {
            // Clean up and rethrow.
            delete heap_int;
            heap_int = nullptr;
            throw; // rethrow the caught exception, only works inside a catch block
        }
    } catch (...) {
        std::cout << "heap_int: " << heap_int << std::endl;
    }
    // Or
    try {
        try {
            throw "Blah";
        } catch (...) {
            // convert the exception into a different type
            throw std::invalid_argument("Blah");
        }
    } catch (std::invalid_argument& error) {
        std::cout << "Caught invalid_argument: " << error.what() << std::endl;
    }
}

static int FooId = 0;
struct Foo {
    int id = ++FooId;

    Foo() {
        std::cout << "Constructed Foo-" << id << std::endl;
    }

    Foo(const Foo& other) {
        // Increment the id instead of copying it.
        std::cout << "Copy-constructed Foo-" << id << " from Foo-" << other.id << std::endl;
    }

    Foo(const Foo&& other)
        : id(other.id) {
        // Copy the id.
        std::cout << "Move-constructed Foo-" << id << std::endl;
    }

    Foo& operator=(Foo&& other) {
        std::cout << "Moved Foo-" << other.id << std::endl;
        // The left hand side id here is zero initialized.
        id = other.id;
        return *this;
    }

    ~Foo() {
        std::cout << "Destroyed Foo-" << id << std::endl;
    }
};

// When you design a function's interface, decide what it's allowed to throw.
// Be careful about changing what a function can throw.
void g() {
    Foo foo1;
    if (true) {
        throw std::invalid_argument("Something bad happened");
        // - executes the destructors for g's local objects
        // - transfers control back to g's caller in search of a handler
    }
    Foo foo2;
}

void f() {
    try {
        g(); // not catching invalid_argument here
    } catch (std::out_of_range& error) {
        std::cout << "Caught out_of_range in f(): " << error.what() << std::endl;
    } catch (std::overflow_error& error) {
        std::cout << "Caught overflow_error in f(): " << error.what() << std::endl;
    }
    // if f has no exception handlers or no matching exception handlers,
    // its local objects are also destroyed and the control is transferred
    // to the f's caller
}

// noexcept is not enforced at compile time.
// If a noexcept function actually throws, std::terminate is called.
void can_not_throw() noexcept {
    std::cout << "Can't throw, sorry." << std::endl;
    // throw "No, really, can't!";
}

// Code is called "exception neutral" when it's set up in such a way that it
// doesn't know if an exception is thrown or not, but it will respond appropriately
// regardless (for example, rethrow).

// Code is called "exception safe" if it keeps the program in a consistent state
// even if a throw occurs.

// Types of guarantees:
//
// - Basic
//
//   The program state may change, but it will remain self-consistent,
//   that is to say a function that throws won't cause a resource leak
//   and won't leave an object in an invalid state.
//
// - Strong
//
//   If a function throws an exception the program's state remains unchanged
//   (as if a function wasn't called). That is to say a function has a
//   "commit or rollback" semantics.
//
// - Nothrow
//
//   No exceptions will ever propagate from the function.
//   If a throw occurs during function execution, the function will catch
//   the exception and deal with it within the function.

// Failure in constructors.
//
// When constructor throws an exception it is interrupted before it finishes.
// The destructor never runs, so the constructor needs to clean up resources
// that were allocated before the throw. Ideally such resources would be wrapped
// into their own classes, so that the cleanup would happen automatically.

const char* get_text() {
    throw std::logic_error("Whoops!");
    return "9000";
}

void basic_guarantee() {
    std::ofstream output("output.txt");
    if (output.is_open()) {
        output << "The value is: ";
        // If get_text() throws, the output will contain 'T' (for some reason).
        // The outcome might depend on environment.
        // output << get_text();

        // So we do this instead:
        try {
            output << get_text();
        } catch (...) {
            // Rethrowing without closing first
            // will also make the output contain 'T'.
            output.close();
            throw;
        }
    } else {
        std::cout << "Couldn't open file" << std::endl;
    }
}

int main(int, char**) {
    try {
        throw_everything();
    } catch (std::exception& error) {
        std::cout << "Caught in main(): " << error.what() << std::endl;
    }

    std::cout << "--------------------" << std::endl;

    catch_everything();

    std::cout << "--------------------" << std::endl;

    try {
        f();
    } catch (std::invalid_argument& error) {
        std::cout << "Caught invalid_argument in main(): " << error.what() << std::endl;
    }

    // An exception can propagate from main() if main() itself or
    // a function it calls throws an exception that main doesn't catch.
    // When this happens, the program calls std::terminate().

    return 0;
}
