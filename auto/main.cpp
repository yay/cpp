#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

std::string getName() {
    return "Vitaly";
}

class Device {};

class DeviceManager {
private:
    std::unordered_map<std::string, std::vector<Device*>> m_devices;
public:
    const std::unordered_map<std::string, std::vector<Device*>> &getDevices() const {
        return m_devices;
    }
};

int main() {
    int a {5};
    auto i {5};         // int
    auto str {"hello"}; // const char *str
    auto b {4.0};       // double

    auto name = getName();

    std::vector<std::string> strings { "Apple", "Orange" };

    for (std::vector<std::string>::iterator it = strings.begin();
        it != strings.end(); it++) {
            std::cout << *it << std::endl;
        }

    for (auto it = strings.begin(); it != strings.end(); it++) {
        std::cout << *it << std::endl;
    }

    DeviceManager dm;
    const std::unordered_map<std::string, std::vector<Device*>> devices = dm.getDevices();

    // C++ syntax
    using DeviceMap = std::unordered_map<std::string, std::vector<Device*>>;
    // C syntax
    typedef std::unordered_map<std::string, std::vector<Device*>> DeviceMap2;

    const DeviceMap &deviceMap = dm.getDevices();
    const DeviceMap2 &deviceMap2 = dm.getDevices();

    // If we don't use the `&` below, `getDevices` will return a copy.
    const auto &someDevices = dm.getDevices();
}
