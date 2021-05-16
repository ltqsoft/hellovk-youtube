#ifndef APP_HH
#define APP_HH

#include "data.hh"
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#define ERR_GLFW_INIT           1
#define ERR_GLFW_WINDOW         2
#define ERR_VK_DEB_CALLBACK     3
#define ERR_VK_PRESENT_SUPPORT  4
#define ERR_VK_SHADER_MOD       5
#define ERR_VK_MEMORY_TYPE      6
#define ERR_TEXTURE_FILE_LOAD   7
#define ERR_MODEL_LOAD_FAIL     8
#define ERR_MAT_NO_TEXTURE      9

#define vkfunc(result, errorMsg, expr) \
    result = expr;  \
    if(result != VK_SUCCESS) throw rterr(result, errorMsg);

typedef std::pair<int, std::runtime_error> rterr;


class Application
{
    void init();
    
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

    const VkSampleCountFlagBits msaaLevel = VK_SAMPLE_COUNT_1_BIT;

    Image depthImage;
    const VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
    void createDepthImage();
    Image createImage(
        uint32_t width, uint32_t height, VkFormat format,
        VkImageUsageFlags usage, VkImageAspectFlags aspect
    );

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainViews;
    void createSwapchainViews();
    VkImageView createImageView(
        VkImage image, VkFormat format, VkImageAspectFlags aspect
    );

    VkRenderPass renderPass = VK_NULL_HANDLE;
    const size_t 
        SO_LUONG_ATTACHMENT = 2,
        ATTACH_COLOR = 0,
        ATTACH_DEPTH = 1
        ;
    void createRenderPass();

    std::vector<VkFramebuffer> framebuffers;
    void createFramebuffers();

    VkCommandPool cmdPool;
    std::vector<VkCommandBuffer> cmds;
    void createCmdPools();
    void createCmds();

    VkFence imageReadyFence;
    VkFence renderDoneFence;
    void createSyncObjects();
    
    void keyboard();
    void update();
    void draw();

    Buffer ubo1;
    glm::mat4 projMatrix;
    Buffer ubo2;
    UniformBufferData ubo2Data;
    void createUniformBuffers();
    Buffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
    Buffer createTempBuffer(VkDeviceSize size, void* data);
    uint32_t getMemType(uint32_t typeBits, VkMemoryPropertyFlags properties);

    Image loadTexture(const char* const filePath);

    VkDescriptorSetLayout dsLayout;
    const uint32_t  SO_LUONG_BINDING = 3,
                    SO_LUONG_UNIFORM_BUFFER = 2,
                    SO_LUONG_SAMPLER = 1,
                    BIND_ID_PROJECTION_MATRIX = 0,
                    BIND_ID_UBO2 = 1,
                    BIND_ID_TEXTURE = 2
                    ;
    void createDescriptorSetLayout();

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    void createDescriptorPool();

    VkShaderModule vertMod;
    VkShaderModule fragMod;
    void loadShaders();
    VkShaderModule createShaderMod(const char* filepath);

    PushConstData pushData;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    void createPipelineLayout();

    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    void createGraphicsPipeline();

    static Viewer viewer;
    void setupViewer();

    VkCommandPool copyPool;
    VkCommandBuffer copyCmd;
    void beginCopy();
    void endCopy();

    const uint32_t
        SO_LUONG_VERTEX_ATTR = 3,
        ATTR_POSITION = 0,
        ATTR_COLOR = 1,
        ATTR_TEXTURE_COORD = 2
        ;

    std::vector<Mesh> meshes;
    void loadModel();

    void cleanup();

public:
    Application();
    ~Application();
};

#endif
