#include <iostream>
#include <array>
#include <vector>
#include <functional>
#include <algorithm> // for_each

// https://shaharmike.com/cpp/lambdas-and-functions/

void lambdaSyntax() {
    std::cout << "----- Syntax -----" << std::endl;

    [](){}(); // emply lambda that does nothing

    int i = 0, j = 1;
    // Capture i by value and j by reference.
    auto func = [i, &j](bool b, float f) {
        ++j;
        // using `boolalpha` to display booleans as true/false instead of 1/0
        // using `fixed` to use fixed floating point notation (1.0f will be displayed as 1.000000 instead of 1)
        std::cout << std::boolalpha << std::fixed << i << ", " << b << ", " << f << std::endl; // 0, true, 1.000000
    };
    func(true, 1.0f);
    std::cout << j << std::endl; // 2

    // Lambda's captures are data members, the lambda can access these members inside its code scope.
    // When a lambda is created, a constructor copies the captured variables to the data members.
    // It has an `operator()(...)`, in this case the `...` is `bool, float`.
    // It has a scope lifetime and a destructor that frees members.

    // You can also specify a default capture.

    // Capture everything by reference.
    [&]() { i = 0; j = 0; }();
    std::cout << i << ", " << j << std::endl; // 0, 0

    int k = 5;
    [=]() { // Capture everything by value. Be careful of the overheads.
        // i = 2; // ERROR: cannot assign to a variable captured by copy in a non-mutable lambda
        // j = 2; // ERROR
        std::cout << k << std::endl; // 5
    }();

    // capture all variables by reference except i and j
    [&, i, j]() {
        k = 10;
    }();
    std::cout << k << std::endl; // 10

    // capture all variables by value except k
    [=, &k]() {
        k = 11;
    }();
    std::cout << k << std::endl; // 11

    // Default captures capture all variables that are both
    // a) not expressly named and
    // b) _used_ in the body of the lambda expression

    // Lambda with return type:
    {
        auto l = [](int a) -> float { return a - 1; };
        float v = l(6);
        std::cout << v << std::endl; // 5.000000
    }

    // The return type may be omitted if:
    // - the return type is void
    // - the compiler can deduce the return type
    {
        auto l = [](int a) { return a - 1; }; // deduced type is int
        auto av = l(6); // av is int
        float v = l(6); // this is still valid though (type will be narrowed to float)
    }
}


void lambdaType() {
    std::cout << "----- Lambda’s type -----" << std::endl;

    // A lambda is not an `std::function`.
    // There is actually no standard type for lambdas.
    // A lambda type is compiler-generated and implementation defined;
    // its only known by the compiler, and the only way to capture a lambda
    // with no conversion is by using `auto`:

    int i = 5;
    auto f = [i](){};
    void (*bar)();
    // bar = f; // ERROR: assigning from incompatible type

    // However, if your capture list is empty, you may convert your lambda
    // to a C-style function pointer:
    void (*foo)(bool, int);
    foo = [](bool, int) {};
}

void lambdaScope() {
    std::cout << "----- Lambda's scope -----" << std::endl;
    // All captured variables have the scope of the lambda.

    struct MyStruct {
        MyStruct() { std::cout << "Constructed" << std::endl; }
        MyStruct(MyStruct const &) { std::cout << "Copy-Constructed" << std::endl; }
        ~MyStruct() { std::cout << "Destructed" << std::endl; }
    };

    std::cout << "Creating MyStruct..." << std::endl;
    MyStruct ms;

    {
        std::cout << "Creating lambda..." << std::endl;
        auto f = [ms]() {}; // `ms` is captured by value
        std::cout << "Destroying lambda..." << std::endl;
    }

    std::cout << "Destroying MyStruct..." << std::endl;

    // Output:

    // Creating MyStruct...
    // Constructed
    // Creating lambda...
    // Copy-Constructed
    // Destroying lambda...
    // Destructed
    // Destroying MyStruct...
    // Destructed
}


void mutableLambdas() {
    std::cout << "----- Mutable lambdas -----" << std::endl;
    // Lambda's `operator()` is const by default - it can't modify the variables captured by value
    // (which is analogous to class members). To change this default add `mutable`:

    {
        int i {1};
        [&i]() { i = 2; }();        // OK
        // [i]() { i = 3; };        // ERROR
        [i]() mutable { i = 5; }(); // OK
    }

    // This gets even more interesting when talking about copying lambdas.
    // Key thing to remember - they behave like classes:
    {
        int i = 0; // this will never be modified
        auto x = [i]() mutable { std::cout << ++i << std::endl; };
        x(); // 1
        std::cout << i << std::endl; // 0
        auto y = x; // data members are copied from x to y, x's `i` already has a value of 1
        x(); // 2
        y(); // 2
        std::cout << i << std::endl; // 0
    }
}

void lambdaSize() {
    std::cout << "----- Lambda’s size -----" << std::endl;

    // Because lambdas have captures, there's no single size for all lambdas:

    auto f1 = [](){ return 42; }; // lambda with no state = functor with no member variables:
    struct Functor42 {
        int operator()() { return 42; }
    } functor;
    // Lambdas are objects, minimum object size in C++ is 1.
    std::cout << sizeof(f1) << std::endl; // 1
    std::cout << sizeof(functor) << std::endl; // 1

    // The structs/classes with methods but no state have the minimum size of 1 because
    // the methods themselves are not a part of the struct/class. They are static functions
    // that implicitly take a pointer to `this` as their first parameter.

    std::array<char, 100> arr;
    auto f2 = [&arr](){};
    std::cout << sizeof(f2) << std::endl; // 8

    auto f3 = [arr](){};
    std::cout << sizeof(f3) << std::endl; // 100
}


void performance() {
    // Lambdas are also awesome when it comes to performance.
    // Because they are objects rather than pointers, they can be inlined
    // very easily by the compiler, much like functors. This means that calling a lambda many times
    // (such as with `std::sort` or `std::copy_if`) is much better than using
    // a global function. This is one example where C++ is actually faster than C.
}


class X {
public:
    void op() { std::cout << "X::op()" << std::endl; }
};

// A functor is a class/struct that provides an implementation of operator().
class XFunctor {
public:
    void operator()(X &elem) { elem.op(); }
};

void functors() {
    std::cout << "----- Functors -----" << std::endl;

    std::vector<X> vec;
    vec.reserve(2);
    vec.emplace_back(X());
    vec.emplace_back(X());
    XFunctor xf;

    for_each(vec.begin(), vec.end(), xf);
    // Output:
    // X::op()
    // X::op()

    for_each(vec.begin(), vec.end(), [](X &elem) { elem.op(); }); // lambda is scoped to for_each
    // Output:
    // X::op()
    // X::op()

    // When a lambda is defined, compiler uses that information to generate
    // an ad-hoc functor class, and the client code is modified to use the generated
    // functor:

    // for_each(vec.begin(), vec.end(), _SomeCompilerGeneratedFunctorName_);
}


void lambdaLifetime() {
    std::cout << "----- Lambda Lifetime -----" << std::endl;
    // A lambda is, as we’ve seen, just an object and, like other objects it may be copied,
    // passed as a parameter, stored in a container, etc. The lambda object has its own scope
    // and lifetime which may, in some circumstances, be different to those objects it has ‘captured’.
    // Be very careful when capturing local objects by reference because a lambda’s lifetime
    // may exceed the lifetime of its capture list.

    auto l = [](){
        int x{5}, y{7};
        float a(1.0f), b(2.0f);
        return [&, x, y](){
            std::cout << "x: " << x << ", y: " << y << ", a: " << a << ", b: " << b << std::endl;
        };
    }();

    // Prints some random values for `a` and `b` because they were captured by reference
    // but are no longer exist:

    l(); // x: 5, y: 7, a: 0.000000, b: 998223950905344.000000

    /* In JavaScript this would have worked just fine:

    var lamb = (function outer() {
        var x = 5,
            y = 7,
            a = 1.0,
            b = 2.0;
        return function inner() { console.log(x, y, a, b); };
    })();
    lamb(); // 5 7 1 2

    When the `outer` function returns, its stack unwinds: the x, y, a, b variables
    are popped off the stack and moved into the heap, where the `inner` function
    can still access them.

    */
}

void underTheHood() {
    std::cout << "----- Under the Hood -----" << std::endl;

    {
        int x{5}, y{7};
        float a{1.0f}, b{2.0f};

        auto l = [&, x, y](double d) {
            a += 1.0f;
            b -= 1.0f;
            std::cout << "x: " << x << ", y: " << y
                    << ", a: " << a << ", b: " << b
                    << ", d: " << d
                    << std::endl;
        };

        l(1.5);
        l(2.5);
        l(3.5);
    }

    std::cout << " --- --- --- " << std::endl;

    {
        int x{5}, y{7};
        float a{1.0f}, b{2.0f};

        class _CompilerGeneratedFunctorName_ {
        public:
            _CompilerGeneratedFunctorName_(int x, int y, float &a, float &b)
                : x_{x}, y_{y}, a_{a}, b_{b} {}
            void operator() (double d) const {
                a_ += 1.0f;
                b_ -= 1.0f;
                std::cout << "x: " << x_ << ", y: " << y_
                    << ", a: " << a_ << ", b: " << b_
                    << ", d: " << d
                    << std::endl;
            }
        private:
            int x_, y_;
            float &a_, &b_;
        };

        _CompilerGeneratedFunctorName_ f(x, y, a, b);

        f(1.5);
        f(2.5);
        f(3.5);
    }

    // Output:

    // x: 5, y: 7, a: 2.000000, b: 1.000000, d: 1.500000
    // x: 5, y: 7, a: 3.000000, b: 0.000000, d: 2.500000
    // x: 5, y: 7, a: 4.000000, b: -1.000000, d: 3.500000
    // --- --- ---
    // x: 5, y: 7, a: 2.000000, b: 1.000000, d: 1.500000
    // x: 5, y: 7, a: 3.000000, b: 0.000000, d: 2.500000
    // x: 5, y: 7, a: 4.000000, b: -1.000000, d: 3.500000
}


void global_f() {
    std::cout << "global_f()" << std::endl;
}

struct Functor {
    void operator()() { std::cout << "Functor" << std::endl; }
};

void stdFunction() {
    std::cout << "----- std::function -----" << std::endl;
    // std::function is a templated object that is used to store and call any callable type,
    // such as functions, objects, lambdas and the result of std::bind.

    std::function<void()> f;
    std::function<double(int,float,double)> foo;
    std::cout << "sizeof(f): " << sizeof(f) << std::endl;   // 48
    std::cout << "sizeof(f): " << sizeof(foo) << std::endl; // 48

    // On clang++ the size of all std::function is always 48 bytes (used to be 32 as of early 2016).
    // It uses what is called `small size optimization`, much like std::string does on many
    // implementations. This basically means that for small objects std::function can keep them
    // as part of its memory, but for bigger objects it defers to dynamic memory allocation.

    class A {
    public:
        void member() { std::cout << "member function" << std::endl; }
    } a;

    f = global_f;
    f(); // global_f()

    f = [](){ std::cout << "Lambda" << std::endl; };
    f(); // Lambda

    Functor functor;
    f = functor;
    f(); // Functor

    // std::function provides an overloaded operator `!=` to allow it to be compared to `nullptr`,
    // so it can act as a function pointer.
    if (f != nullptr) {
        f();
    }
}

void insideMemberFunctions() {
    std::cout << "----- Lambdas within member functions -----" << std::endl;

    // It is perfectly possible to use lambdas inside class member functions.
    // But since lambda is a unique (and separate) class of its own,
    // when it executes it has its own context. Therefore, it does not have direct access
    // to any of the class’ member variables.

    // To capture the class’ member variables we must capture the `this` pointer of the class.
    // We now have full access to all the class’ data (including private data,
    // as if we were inside a member function).

    class Filter {
    public:
        Filter(std::vector<int> src) : v(src) {}

        void filter() {
            remove_if(v.begin(), v.end(), [this](int i) -> bool { return i < level; });
        }
    private:
        std::vector<int> v;
        int level;
    };

    // Note that capturing this by reference doesn’t really make sense
    // (you can’t change its value), so it should always appear in a capture statement
    // as capturing by value.

    // The value of `this` can't be changed because it's a const pointer:

    class A {
        int a = 0;
        // `this` is of type `A *const`
        void f() { std::cout << this->a++ << std::endl; }
    };
}

int main() {
    lambdaSyntax();
    lambdaType();
    lambdaScope();
    mutableLambdas();
    lambdaSize();
    functors();
    lambdaLifetime();
    underTheHood();
    stdFunction();
    insideMemberFunctions();
}
