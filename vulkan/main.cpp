#include <iostream>
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

// #include "httplib.h"

// https://vulkan.lunarg.com/doc/sdk/1.1.121.1/mac/tutorial/html/01-init_instance.html
// https://github.com/Rust-SDL2/rust-sdl2/pull/1030
void initVulkan() {
    VkInstance instance;
    VkInstanceCreateInfo instanceInfo{};

    VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
    if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cerr << "Cannot find a compatible Vulkan ICD\n";
        std::exit(-1);
    } else if (result != VK_SUCCESS) {
        std::cerr << "vkCreateInstance error: " << result << std::endl;
        std::exit(-1);
    }

    std::cout << "vkCreateInstance result: " << result << "\n";

    uint32_t gpuCount;
    if (vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr) == VK_SUCCESS) {
    }

    vkDestroyInstance(instance, nullptr);
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        return true;
    }
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
}

void printSDLError() {
    SDL_Log("SDL error: %s", SDL_GetError());
}

SDL_Window* createWindow(const char* title, int width, int height) {
    return SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
}

// void startServer() {
//     httplib::Server svr;

//     svr.Get("/hi", [](auto, httplib::Response& res) {
//         res.set_content("Hello world!", "text/plain");
//     });

//     svr.listen("0.0.0.0", 8080);
// }

int main(int argc, const char* argv[]) {
    // initVulkan();
    if (initSDL()) {
        auto window = createWindow("Hello", 800, 600);
        if (!window) {
            std::cerr << "Failed to create a window\n";
            printSDLError();
            SDL_Quit();
        }
    }

    return 0;
}
