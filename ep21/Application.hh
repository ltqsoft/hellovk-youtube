#ifndef APP_HH
#define APP_HH

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#define ERR_GLFW_INIT           1
#define ERR_GLFW_WINDOW         2
#define ERR_VK_DEB_CALLBACK     3
#define ERR_VK_PRESENT_SUPPORT  4
#define ERR_VK_SHADER_MOD       5
#define ERR_VK_MEMORY_TYPE       6

#define vkfunc(result, errorMsg, expr) \
    result = expr;  \
    if(result != VK_SUCCESS) throw rterr(result, errorMsg);

typedef std::pair<int, std::runtime_error> rterr;


struct Buffer {
    VkBuffer id;
    VkDeviceMemory mem;
    void* mapData;
};


struct UniformBufferData {
    glm::mat4 worldMatrix;
};


class Application
{
    GLFWwindow* wnd = nullptr;
    int wndWidth = 1024, wndHeight = 768;
    void createWindow();
    
    VkInstance instance = VK_NULL_HANDLE;
    void createInstance();

    PFN_vkCreateDebugUtilsMessengerEXT createDeb = NULL;
    PFN_vkDestroyDebugUtilsMessengerEXT destroyDeb = NULL;
    PFN_vkDebugUtilsMessengerCallbackEXT debCallback;
    VkDebugUtilsMessengerEXT deb = VK_NULL_HANDLE;
    void setupDebugger();

    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceMemoryProperties memProps;
    void pickPhysicalDevice();

    uint32_t queueFamily = ~0;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
    void createLogicalDevice();

    uint32_t minImageCount = 0;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    void createSwapchain();

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainViews;
    void createSwapchainViews();
    VkImageView createImageView(
        VkImage image, VkFormat format, VkImageAspectFlags aspect
    );

    VkRenderPass renderPass = VK_NULL_HANDLE;
    void createRenderPass();

    std::vector<VkFramebuffer> framebuffers;
    void createFramebuffers();

    VkCommandPool cmdPool;
    std::vector<VkCommandBuffer> cmds;
    void createCmds();

    VkFence imageReadyFence;
    VkFence renderDoneFence;
    void createSyncObjects();
    
    void update();
    void draw();

    Buffer ubo1;
    glm::mat4 projMatrix;
    Buffer ubo2;
    UniformBufferData ubo2Data;
    void createUniformBuffers();
    Buffer createUniformBuffer(VkDeviceSize size);
    uint32_t getMemType(uint32_t typeBits, VkMemoryPropertyFlags properties);

    VkDescriptorSetLayout dsLayout;
    const uint32_t  SO_LUONG_BINDING = 2,
                    BIND_ID_PROJECTION_MATRIX = 0,
                    BIND_ID_UBO2 = 1
                    ;
    void createDescriptorSetLayout();

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    void createDescriptorSet();

    VkShaderModule vertMod;
    VkShaderModule fragMod;
    void loadShaders();
    VkShaderModule createShaderMod(const char* filepath);

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    void createPipelineLayout();

    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    void createGraphicsPipeline();

    void cleanup();

public:
    Application();
    ~Application();
};

#endif
