#include <iostream>
#include <chrono>

#include "simdjson.h"

auto time(void block()) -> void {
    auto then = std::chrono::high_resolution_clock::now();
    block();
    auto now = std::chrono::high_resolution_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(now - then);

    std::cout << "Time elapsed: " << seconds.count() << "s" << std::endl;
}

int main() {
    time([]() {
        simdjson::dom::parser parser;
        simdjson::dom::element tweets = parser.load("../twitter.json");
        std::cout << tweets["search_metadata"]["count"] << " results." << std::endl;
    });

    return 0;
}
