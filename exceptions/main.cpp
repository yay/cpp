#include <exception>
#include <stdexcept>
// Some other standard exceptions are declared here:
// #include <new>
// #include <typeinfo>
#include <iostream>
#include <string>
#include <tuple>

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

int main(int, char**) {
    try {
        throw_everything();
    } catch (std::exception& error) {
        std::cout << "Caught: " << error.what() << std::endl;
    }
    return 0;
}
