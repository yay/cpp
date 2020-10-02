#pragma once

#include <vulkan/vulkan_core.h>

#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

// Almost every operation in Vulkan, anything from drawing to uploading textures,
// requires commands to be submitted to a queue. There are different types of queues
// that originate from different queue families and each family of queues allows only a subset of commands.
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

    // The image that we have to use for the attachment depends on which image the swap chain returns
    // when we retrieve one for presentation. That means that we have to create a framebuffer for all
    // of the images in the swap chain and use the one that corresponds to the retrieved image at drawing time.
    std::vector<VkFramebuffer> swapChainFramebuffers;

    // Commands in Vulkan, like drawing operations and memory transfers, are not executed directly
    // using function calls. You have to record all of the operations you want to perform
    // in command buffer objects. The advantage of this is that all of the hard work of setting up
    // the drawing commands can be done in advance and in multiple threads.
    // After that, you just have to tell Vulkan to execute the commands in the main loop.
    // Command pools manage the memory that is used to store the buffers,
    // and command buffers are allocated from them.
    VkCommandPool commandPool;

    // Because one of the drawing commands involves binding the right VkFramebuffer,
    // we'll actually have to record a command buffer for every image in the swap chain once again.
    // Command buffers will be automatically freed when their command pool is destroyed.
    std::vector<VkCommandBuffer> commandBuffers;

    // Each frame should have its own set of semaphores.
    // In Vulkan semaphores are used for GPU-GPU synchronization.
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    // In Vulkan fences are used for CPU-GPU synchronization.
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    // To use the right pair of semaphores every time, we need to keep track of the current frame.
    size_t currentFrame = 0;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkDebugUtilsMessengerEXT debugMessenger;

    void setupWindow();
    void setupVulkan();

    void mainLoop();
    void drawFrame();

    void cleanupVulkan();
    void cleanupWindow();

    void createSwapChain();
    void recreateSwapChain();
    void destroySwapChain();

    void createInstance();
    void destroyInstance();

    void createSurface();
    void destroySurface();
    void pickPhysicalDevice();

    void setupDebugMessenger();
    void cleanupDebugMessenger();

    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createLogicalDevice();
    void destroyLogicalDevice();

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

    VkShaderModule createShaderModule(const std::vector<char>& code);
    bool checkValidationLayerSupport();
};