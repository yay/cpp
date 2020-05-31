// #define FMT_HEADER_ONLY
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
// #include <fmt/format.h>

using json = nlohmann::json;

// https://github.com/nlohmann/json

void create_json() {
    json j; // create an empty structure
    j["pi"] = 3.1415926; // add a number that is stored as double
    j["happy"] = true; // add a Boolean that is stored as bool
    j["name"] = "Niels"; // add a string that is stored as std::string
    j["nothing"] = nullptr;
    j["answer"]["everything"] = 42;
    j["list"] = { 1, 0, 2 }; // add an array that is stored as std::vector
    j["object"] = { { "currency", "USD" }, { "value", 42.99 } };

    json j2 {
        {"pi", 3.1415926},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
            {"currency", "USD"},
            {"value", 42.99}
        }}
    };

    std::cout << j.dump() << std::endl;
    std::cout << j2.dump(4) << std::endl;
}

bool print_file() {
    std::ifstream file {"hey.json"};

    if (!file) {
        std::cerr << "Couldn't open file for reading." << std::endl;
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        std::cout << line << std::endl;
    }

    return true;
}

std::string read_file(const std::string& name) {
    std::ifstream file {name};
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

void parse_json() {
    auto str = read_file("hey.json");
    json j = json::parse(str);

    std::cout << j["surname"] << std::endl;
    std::cout << j["age"] << std::endl;

    json j2 = "{ \"happy\": true, \"who\": \"Vitaly\" }"_json;
    std::cout << j2["happy"] << std::endl;
    std::cout << j2["who"] << std::endl;

    json j3 = R"(
        {
            "happy": true,
            "pi": 3.1415926
        }
    )"_json;

    std::cout << j3["pi"] << std::endl;
}

int main(int argc, char** argv) {
    // print_file();
    parse_json();
    create_json();
    return 0;
}
