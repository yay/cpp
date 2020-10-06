#pragma once

#include <vulkan/vulkan_core.h>

#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

// Almost every operation in Vulkan, anything from drawing to uploading textures,
// requires commands to be submitted to a queue. There are different types of queues
// that originate from different queue families and each family of queues allows only
// a subset of commands.
struct QueueFamilyIndices {
    // It's possible that the queue families supporting drawing commands
    // and the ones supporting presentation do not overlap.
    std::optional<uint32_t> graphicsFamily;
    // The availability of a presentation queue implies that the swap chain
    // extension must be supported.
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class Triangle {
public:
    // Many drivers and platforms trigger VK_ERROR_OUT_OF_DATE_KHR automatically after a window resize,
    // but it is not guaranteed to happen, so we handle the resize explicitly.
    bool framebufferResized = false;

    void run();

private:
    GLFWwindow* window;

    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;

    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    // Each frame should have its own set of semaphores.
    // In Vulkan semaphores are used for GPU-GPU synchronization.
    std::vector<VkSemaphore> imageAvailableSemaphores; // image has been acquired and is ready for rendering
    std::vector<VkSemaphore> renderFinishedSemaphores; // rendering has finished and presentation can happen
    // In Vulkan fences are used for CPU-GPU synchronization.
    // The state of fences can be accessed from by the program via vkWaitForFences,
    // and the state of semaphores cannot be.
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    // To use the right pair of semaphores every time, we need to keep track of the current frame.
    size_t currentFrame = 0;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkDebugUtilsMessengerEXT debugMessenger;

    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VkShaderModule createShaderModule(const std::vector<char>& code);
    bool checkValidationLayerSupport();

    void setupWindow();
    void cleanupWindow();

    void setupVulkan();
    void cleanupVulkan();

    void createInstance();
    void destroyInstance();

    void setupDebugMessenger();
    void cleanupDebugMessenger();

    void createSurface();
    void destroySurface();
    void pickPhysicalDevice();

    void createLogicalDevice();
    void destroyLogicalDevice();

    void createSwapChain();
    void recreateSwapChain();
    void destroySwapChain();

    void createImageViews();
    void destroyImageViews();

    void createRenderPass();
    void destroyRenderPass();

    void createGraphicsPipeline();
    void destroyGraphicsPipeline();

    void createPipelineLayout(VkDevice device);
    void destroyPipelineLayout();

    void createFramebuffers();
    void destroyFramebuffers();

    void createCommandPool();
    void destroyCommandPool();

    void createCommandBuffers();

    void createSyncObjects();
    void destroySyncObjects();

    void mainLoop();
    void drawFrame();
};