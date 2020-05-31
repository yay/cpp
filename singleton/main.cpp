#include <iostream>

class Singleton {
  public:
    // delete the copy-constructor, so that
    // Singleton instance = Singleton::get();
    // doesn't produce a copy.
    Singleton(const Singleton &) = delete;
    // a way to access this class statically
    static auto get() -> Singleton & {
        static Singleton instance;
        return instance;
    }

    auto getValue() -> float { return value; }

  private:
    // make the constructor private to prevent instantiation
    Singleton() {}

    float value = 0.5;
};

auto main() -> int { std::cout << Singleton::get().getValue() << std::endl; }