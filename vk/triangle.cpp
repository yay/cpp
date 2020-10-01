#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>

#include "triangle.h"

// How many frames should be processed concurrently.
const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// Validation layers are optional components that hook into Vulkan function calls
// to apply additional operations.
const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

// The swap chain is a queue of images that are waiting to be presented to the screen.
// The general purpose of the swap chain is to synchronize the presentation of images
// with the refresh rate of the screen. Since presentation is platform specific this
// feature has to be loaded as an extension.
//
// Types of extensions:
// KHR - extensions that are likely to be included into the Vulkan spec
//       or that are supported by a broad range of vendors and hardware
// KHX - experimental versions of KHR extensions
// EXT - aren't vendor specific but are targeting some less common use case
// NV, AMD, etc. - vendor specific extensions
const std::vector<const char*> requiredExtensionNames = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

// Checking if a swap chain is available is not sufficient
// because it may not actually be compatible with our window surface.
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;      // min/max # of images in swap chain, min/max width/height of images
    std::vector<VkSurfaceFormatKHR> formats;    // pixel format, color space
    std::vector<VkPresentModeKHR> presentModes; // presentation modes

    SwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &this->capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            this->formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, this->formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            this->presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount,
                                                      this->presentModes.data());
        };
    }
};

std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open file: ") + filename);
    }

    // The advantage of starting to read at the end of the file
    // is that we can use the read position to determine the size of the file
    // and allocate a buffer:
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    // Seek back to the beginning of the file and read all of the bytes at once.
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void* pUserData) {

    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger) {
    // vkCreateDebugUtilsMessengerEXT is an extension function and is not
    // automatically loaded, so we have to look up its address manually.
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

// https://vulkan-tutorial.com/en/Drawing_a_triangle/Setup/Validation_layers

// Use a static function as a callback is because GLFW does not know how to properly call
// a member function with the right 'this' pointer to our application instance.
// However, we do get a reference to the GLFWwindow in the callback
// and there is another GLFW function that allows us to store an arbitrary pointer
// inside of it: glfwSetWindowUserPointer.
static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Triangle*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Triangle::run() {
    setupWindow();
    setupVulkan();

    mainLoop();

    cleanupVulkan();
    cleanupWindow();
}

void Triangle::setupWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Triangle::cleanupWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Triangle::setupVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffers();
    createSyncObjects();
}

void Triangle::cleanupVulkan() {
    destroySwapChain();
    destroySyncObjects();
    destroyCommandPool();
    destroyLogicalDevice();
    cleanupDebugMessenger();
    destroySurface();
    destroyInstance();
}

void Triangle::recreateSwapChain() {
    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) { // window minimized
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    // Wait on the host for the completion of outstanding queue operations
    // for all queues on a given logical device.
    vkDeviceWaitIdle(device);

    destroySwapChain();
    createSwapChain();

    // The image views need to be recreated because they are based directly on the swap chain images.
    createImageViews();
    // The render pass needs to be recreated because it depends on the format of the swap chain images.
    createRenderPass();
    // Viewport and scissor rectangle size is specified during graphics pipeline creation,
    // so the pipeline also needs to be rebuilt.
    createGraphicsPipeline();
    // The framebuffers and command buffers also directly depend on the swap chain images.
    createFramebuffers();
    createCommandBuffers();
}

void Triangle::destroySwapChain() {
    destroyFramebuffers();
    // Instead of recreating the command pool from scratch simply clean the existing one up.
    vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    destroyGraphicsPipeline();
    destroyPipelineLayout();
    destroyRenderPass();
    destroyImageViews();
    // Swap chain images are automatically cleaned up once the swap chain has been destroyed.
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void Triangle::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested but not available!");
    }

    // Optional data.
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    // Tells the Vulkan driver which global extensions and validation layers we want to use.
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Vulkan is a platform agnostic API, which means that you need an extension
    // to interface with the window system.
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // The debugCreateInfo variable is placed outside the if statement
    // to ensure that it is not destroyed before the vkCreateInstance call.
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
        // To enable debugging vkCreateInstance and vkDestroyInstance calls.
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a Vulkan instance!");
    }
}

void Triangle::destroyInstance() {
    vkDestroyInstance(instance, nullptr);
}

void Triangle::createSurface() {
    // This uses different implementation for each supported platform.
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void Triangle::destroySurface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

void Triangle::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

bool Triangle::isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = findQueueFamilies(device);
    bool dedicatedGPU = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    bool swapChainAdequate = false;
    bool extensionsSupported = checkDeviceExtensionSupport(device);
    if (extensionsSupported) {
        // It is important that we only try to query for swap chain support after verifying that the extension is
        // available.
        SwapChainSupportDetails swapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    bool suitable = dedicatedGPU && indices.isComplete() && extensionsSupported && swapChainAdequate;

    std::cout << "Using the following device: " << deviceProperties.deviceName << std::endl;

    return suitable;
}

QueueFamilyIndices Triangle::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        // Since the presentation is a queue-specific feature, the problem is about
        // finding a queue family that supports presenting to the surface we created.
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

bool Triangle::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    std::set<std::string> requiredExtensions(requiredExtensionNames.begin(), requiredExtensionNames.end());

    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

VkSurfaceFormatKHR Triangle::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // If SRGB format/space is not available, settle with the first available format.
    return availableFormats[0];
}

VkPresentModeKHR Triangle::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    // The swap chain is a queue where the display takes an image from the front of the queue
    // when the display is refreshed and the program inserts rendered images at the back of the queue.
    for (const auto& availablePresentMode : availablePresentModes) {
        // When the queue is full, the images that are already queued are replaced with the newer ones.
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    // If the queue is full then the program has to wait.
    return VK_PRESENT_MODE_FIFO_KHR;
}

// The swap extent is the resolution of the swap chain images and it's almost always exactly equal
// to the resolution of the window that we're drawing to.
VkExtent2D Triangle::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    // However, some window managers do allow us to differ here and this is indicated by setting
    // the width and height in currentExtent to a special value: the maximum value of uint32_t.
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

// It's possible to create multiple logical devices from the same physical device.
void Triangle::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        // Don't really need more than one queue because we can create all of the command buffers
        // on multiple threads and then submit them all at once on the main thread
        // with a single low-overhead call.
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensionNames.size());
    createInfo.ppEnabledExtensionNames = requiredExtensionNames.data();

    // An example of a device specific extension is VK_KHR_swapchain,
    // which allows you to present rendered images from that device to windows.
    // It is possible that there are Vulkan devices in the system that lack this ability,
    // for example because they only support compute operations.
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void Triangle::destroyLogicalDevice() {
    vkDestroyDevice(device, nullptr);
}

void Triangle::createSwapChain() {
    SwapChainSupportDetails swapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    // We may sometimes have to wait on the driver to complete internal operations
    // before we can acquire another image to render to, so we request one more than the minimum.
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    // The imageArrayLayers specifies the amount of layers each image consists of.
    // This is always 1 unless you are developing a stereoscopic 3D application.
    createInfo.imageArrayLayers = 1;
    // Render directly to images, which means that they're used as color attachment.
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        // Images can be used across multiple queue families without explicit ownership transfers.
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        // An image is owned by one queue family at a time and ownership must be explicitly
        // transferred before using it in another queue family.
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // best performance
        createInfo.queueFamilyIndexCount = 0;                    // optional
        createInfo.pQueueFamilyIndices = nullptr;                // optional
    }

    // Don't use any transformations.
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    // Ignore the alpha channel.
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    // Don't care about the color of pixels that are obscured,
    // for example because another window is in front of them.
    createInfo.clipped = VK_TRUE;

    // With Vulkan it's possible that your swap chain becomes invalid or unoptimized
    // while your application is running, for example because the window was resized.
    // In that case the swap chain actually needs to be recreated from scratch and a reference
    // to the old one must be specified in this field.
    // https://vulkan-tutorial.com/en/Drawing_a_triangle/Swap_chain_recreation
    // For now we'll assume that we'll only ever create one swap chain.
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a swap chain!");
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void Triangle::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];

        // 1D, 2D, 3D texture or a cube map
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;

        // The components field allows you to swizzle the color channels around.
        // For example, you can map all of the channels to the red channel for a monochrome texture.
        // We stick to the default mapping.
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // Our images will be used as color targets without any mipmapping levels or multiple layers.
        // If you were working on a stereographic 3D application, then you would create a swap chain
        // with multiple layers. You could then create multiple image views for each image representing
        // the views for the left and right eyes by accessing different layers.
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image views!");
        }
    }
}

// Tells Vulkan about the framebuffer attachments that will be used while rendering.
void Triangle::createRenderPass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    // loadOp and storeOp apply to color and depth data and determine what to do with the data
    // in the attachment before rendering and after rendering.
    // Clear the values to a constant (black) at the start.
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    // Rendered contents will be stored in memory and can be read later.
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    // The layout of the pixels in memory can change based on what you're trying to do with an image.
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    // The index of the attachment in this array is directly referenced from the fragment shader with the
    // 'layout(location = 0) out vec4 outColor' directive.
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Triangle::destroyRenderPass() {
    vkDestroyRenderPass(device, renderPass, nullptr);
}

// The graphics pipeline in Vulkan is almost completely immutable,
// so you must recreate the pipeline from scratch if you want to change shaders,
// bind different framebuffers or change the blend function.
// However, because all of the operations you'll be doing in the pipeline are known in advance,
// the driver can optimize for it much better.
void Triangle::createGraphicsPipeline() {
    auto vertShaderCode = readFile("../shaders/triangle-vert.spv");
    auto fragShaderCode = readFile("../shaders/triangle-frag.spv");

    // The compilation and linking of the SPIR-V bytecode to machine code for execution
    // by the GPU doesn't happen until the graphics pipeline is created. That means that
    // we're allowed to destroy the shader modules again as soon as pipeline creation is finished,
    // which is why we'll make them local variables.
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // To actually use the shaders we'll need to assign them to a specific pipeline stage.
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    // It's possible to combine multiple fragment shaders into a single shader module
    // and use different entry points to differentiate between their behaviors.
    vertShaderStageInfo.pName = "main"; // entry point

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // Describes the format of the vertex data that will be passed to the vertex shader.
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    // What kind of geometry will be drawn from the vertices and if primitive restart should be enabled.
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // triangle from every 3 vertices without reuse
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // A viewport describes the region of the framebuffer that the output will be rendered to.
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Draw to the entire framebuffer.
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // The rasterizer takes the geometry that is shaped by the vertices from the vertex shader
    // and turns it into fragments to be colored by the fragment shader.
    // It also performs depth testing, face culling and the scissor test, and it can be configured
    // to output fragments that fill entire polygons or just the edges (wireframe).
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    // Multisampling works by combining the fragment shader results of multiple polygons
    // that rasterize to the same pixel. Because it doesn't need to run the fragment shader multiple times
    // if only one polygon maps to a pixel, it is significantly less expensive than simply rendering
    // to a higher resolution and then downscaling.
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    // After a fragment shader has returned a color, it needs to be combined with the color that
    // is already in the framebuffer. This transformation is known as color blending.
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    // The new color from the fragment shader is passed through unmodified:
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAttachment.blendEnable = VK_TRUE;
    // colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    // colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    // A limited amount of the state that we've specified in the previous structs
    // can actually be changed without recreating the pipeline. Examples are the size of the viewport,
    // line width and blend constants.
    VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    createPipelineLayout(device);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    // Vulkan allows you to create a new graphics pipeline by deriving from an existing pipeline. The idea of
    // pipeline derivatives is that it is less expensive to set up pipelines when they have much functionality in
    // common with an existing pipeline and switching between pipelines from the same parent can also be done
    // quicker.
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    // vkCreateGraphicsPipelines is designed to take multiple VkGraphicsPipelineCreateInfo objects
    // and create multiple VkPipeline objects in a single call.
    // A pipeline cache can be used to store and reuse data relevant to pipeline creation across multiple calls to
    // vkCreateGraphicsPipelines and even across program executions if the cache is stored to a file.
    // This makes it possible to significantly speed up pipeline creation at a later time.
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a graphics pipeline!");
    }

    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void Triangle::destroyGraphicsPipeline() {
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
}

void Triangle::createPipelineLayout(VkDevice device) {
    // You can use uniform values in shaders, which are globals similar to dynamic state variables that can be
    // changed at drawing time to alter the behavior of your shaders without having to recreate them. They are
    // commonly used to pass the transformation matrix to the vertex shader, or to create texture samplers in the
    // fragment shader.
    // These uniform values need to be specified during pipeline creation by creating a VkPipelineLayout object.
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a pipeline layout!");
    }
}

void Triangle::destroyPipelineLayout() {
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
}

void Triangle::createFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        // You can only use a framebuffer with the render passes that it is compatible with,
        // which roughly means that they use the same number and type of attachments.
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a framebuffer!");
        }
    }
}

void Triangle::destroyFramebuffers() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}

void Triangle::createCommandPool() {
    // Command buffers are executed by submitting them on one of the device queues,
    // like the graphics and presentation queues we retrieved.
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    // Each command pool can only allocate command buffers that are submitted on a single type of queue.
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = 0;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a command pool!");
    }
}

void Triangle::destroyCommandPool() {
    vkDestroyCommandPool(device, commandPool, nullptr);
}

void Triangle::createCommandBuffers() {
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    // Can be submitted to a queue for execution, but cannot be called from other command buffers.
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        // If the command buffer was already recorded once, then a call to vkBeginCommandBuffer
        // will implicitly reset it. It's not possible to append commands to a buffer at a later time.
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin recording a command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent;

        // The last two parameters define the clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR,
        // which we used as load operation for the color attachment. We define the clear color
        // to simply be black with 100% opacity.
        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        // All of the functions that record commands can be recognized by their vkCmd prefix.
        // They all return void, so there will be no error handling until we've finished recording.
        // The flag at the end means that the render pass commands will be embedded in the primary
        // command buffer itself and no secondary command buffers will be executed.
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record a command buffer!");
        }
    }
}

void Triangle::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    // By default, fences are created in the unsignaled state.
    // That means that vkWaitForFences will wait forever if we haven't used the fence before.
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("Failed to create synchronization objects for a frame!");
        }
    }
}

void Triangle::destroySyncObjects() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }
}

VkShaderModule Triangle::createShaderModule(const std::vector<char>& code) {
    // The one catch is that the size of the bytecode is specified in bytes,
    // but the bytecode pointer is a uint32_t pointer rather than a char pointer.
    // Therefore we will need to cast the pointer with reinterpret_cast as shown
    // below. When you perform a cast like this, you also need to ensure that the data satisfies
    // the alignment requirements of uint32_t. Lucky for us, the data is stored in an std::vector
    // where the default allocator already ensures that the data satisfies the worst case
    // alignment requirements.
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a shader module!");
    }

    return shaderModule;
}

void Triangle::destroyImageViews() {
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }
}

bool Triangle::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> Triangle::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    // This includes platform-specific extensions needed to create a window surface.
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

#ifndef NDEBUG
    std::cout << "Required extensions:" << std::endl;
    for (const char* extension : extensions) {
        std::cout << extension << std::endl;
    }
    std::cout << std::endl;
#endif

    return extensions;
}

void Triangle::setupDebugMessenger() {
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

void Triangle::cleanupDebugMessenger() {
    if (!enableValidationLayers)
        return;

    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
}

void Triangle::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

void Triangle::drawFrame() {
    // Wait for the frame to be finished.
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // The function calls will return before the operations are actually finished
    // and the order of execution is also undefined. That is unfortunate,
    // because each of the operations depends on the previous one finishing.

    // There are two ways of synchronizing swap chain events: fences and semaphores.
    // The difference is that the state of fences can be accessed from your program
    // using calls like vkWaitForFences and semaphores cannot be. Fences are mainly
    // designed to synchronize your application itself with rendering operation,
    // whereas semaphores are used to synchronize operations within or across command queues.

    uint32_t imageIndex;
    // imageAvailableSemaphore will be signaled when the presentation engine is finished using the image.
    // That's the point in time where we can start drawing to it.
    // The last parameter specifies a variable to output the index of the swap chain image that has become
    // available.
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame],
                                            VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        // The swap chain has become incompatible with the surface and can no longer be used for rendering.
        // Usually happens after a window resize.
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire a swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on).
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame.
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    // Configure queue submission and synchronization.
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    // We want to wait with writing colors to the image until it's available, so we're specifying the stage
    // of the graphics pipeline that writes to the color attachment.
    // Each entry in the waitStages array corresponds to the semaphore with the same index in pWaitSemaphores.
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit a draw command buffer!");
    }

    // Submit the result back to the swapchain.
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        // If the swap chain turns out to be out of date when attempting to acquire an image,
        // then it is no longer possible to present to it.
        // It is important to perform the framebufferResized check after vkQueuePresentKHR
        // to ensure that the semaphores are in a consistent state, otherwise a signalled semaphore
        // may never be properly waited upon.
        framebufferResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present a swap chain image!");
    }

    // Advance to the next frame every time.
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}