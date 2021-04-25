#include "Application.hh"
#include <vector>


Application::Application()
{
    this->createWindow();
    this->createInstance();
    this->setupDebugger();
    this->pickPhysicalDevice();
    this->createLogicalDevice();
    this->createSwapchain();

    while(!glfwWindowShouldClose(wnd))
    {
        glfwPollEvents();
    }

    this->cleanup();
    std::cout << "Chuong trinh ket thuc.\n";
}


Application::~Application()
{}


void Application::cleanup()
{
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    this->destroyDeb(instance, deb, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(wnd);
    glfwTerminate();
}


void Application::createWindow()
{
    glfwSetErrorCallback([](int code, const char* details)
    {
        std::cerr << 
            "GLFW error " << code << ": " << details << '\n';
    });
    if(!glfwInit()) throw rterr(
        ERR_GLFW_INIT, "Loi: glfwInit() khong thanh cong!");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    wnd = glfwCreateWindow(
        wndWidth, wndHeight, "Hello, jackass!", nullptr, nullptr
    );
    if(wnd == nullptr) throw rterr(
        ERR_GLFW_WINDOW, "Loi: Tao cua so Vulkan khong thanh cong!"
    );
    std::cout << "Tao cua so Vulkan thanh cong.\n";
}


void Application::createInstance()
{
    std::vector<const char*> layers;
    layers.push_back("VK_LAYER_KHRONOS_validation");
    uint32_t count = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&count);
    std::vector<const char*> exts;
    exts.resize(count);
    for(std::size_t i=0; i<count; i++) {
        exts[i] = glfwExts[i];
    }
    exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.enabledLayerCount = 
        static_cast<uint32_t>(layers.size());
    instanceInfo.ppEnabledLayerNames = layers.data();
    instanceInfo.enabledExtensionCount = 
        static_cast<uint32_t>(exts.size());
    instanceInfo.ppEnabledExtensionNames = exts.data();

    VkResult result;
    vkfunc(result, "Loi: Tao instance khong thanh cong!",
        vkCreateInstance(&instanceInfo, nullptr, &instance));
    std::cout << "Tao instance thanh cong.\n";
}


void Application::setupDebugger()
{
    createDeb = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance,
            "vkCreateDebugUtilsMessengerEXT");
    if(createDeb == NULL) throw rterr(ERR_VK_DEB_CALLBACK,
        "Loi: khong tim thay ham tao debugger!"
    );
    destroyDeb = (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance,
            "vkDestroyDebugUtilsMessengerEXT"
    );
    if(destroyDeb == NULL) throw rterr(
        ERR_VK_DEB_CALLBACK, "Loi: khong tim thay ham huy debugger!"
    );
    debCallback = [](
        VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
        void*                                            pUserData
    )->VkBool32
    {
        if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            std::cerr << "=Validation ERROR=\n" <<
                pCallbackData->pMessage << '\n';
            exit(ERR_VK_DEB_CALLBACK);
        }
        else std::cout << "=DebCallback=\n" <<
            pCallbackData->pMessage << '\n';

        return VK_FALSE;
    };

    VkDebugUtilsMessengerCreateInfoEXT debInfo = {};
    debInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debInfo.pfnUserCallback = debCallback;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao debugger khong thanh cong!",
        this->createDeb(instance, &debInfo, nullptr, &deb));
    std::cout << "Tao debugger thanh cong.\n";
}


void Application::pickPhysicalDevice()
{
    VkResult result = VK_ERROR_UNKNOWN;
    uint32_t count = 0;
    vkfunc(result, "Loi: thiet bi cua ban khong co GPU ho tro Vulkan!",
        vkEnumeratePhysicalDevices(instance, &count, nullptr);
    );
    std::vector<VkPhysicalDevice> foundDevices;
    foundDevices.resize(count);
    vkEnumeratePhysicalDevices(instance, &count, foundDevices.data());
    std::cout << "Tim  thay " << count << " GPU ho tro Vulkan.\n";
    for(VkPhysicalDevice dev : foundDevices)
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(dev, &props);
        std::cout << '\t' << props.deviceName << '\n';
    }

    physicalDevice = foundDevices[0]; // chon cai dau tien trong danh sach
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
}


void Application::createLogicalDevice()
{

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice, &count, nullptr
    );
    std::vector<VkQueueFamilyProperties> families;
    families.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice, &count, families.data()
    );
    for(size_t i=0; i<count; ++i)
    {
        if(
            families[i].queueCount &&
            families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT
        )
        {
            std::cout << "Queue family index " << i << 
                " co tinh nang graphics.\n";
            queueFamily = i;
            break;
        }
    }

    float queuePrior = 1.0f;
    VkDeviceQueueCreateInfo queueInfo = {};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamily;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePrior;

    std::vector<const char*> exts;
    exts.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(physicalDevice, &features);

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
    deviceInfo.ppEnabledExtensionNames = exts.data();
    deviceInfo.pEnabledFeatures = &features;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao logical device khong thanh cong!",
        vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device);
    );

    vkGetDeviceQueue(device, queueFamily, 0, &queue);

    std::cout << "Tao logical device thanh cong.\n";
}


void Application::createSwapchain()
{
    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao surface that bai!",
        glfwCreateWindowSurface(instance, wnd, nullptr, &surface);
    );

    VkExtent2D extent;
    extent.width = wndWidth;
    extent.height = wndHeight;

    VkSurfaceCapabilitiesKHR surfaceCaps;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        physicalDevice, surface, &surfaceCaps
    );
    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        physicalDevice, queueFamily, surface, &presentSupport
    );
    if(!presentSupport) throw rterr(
        ERR_VK_PRESENT_SUPPORT,
        "Loi: surface khong co tinh nang present cho queue family!"
    );

    VkSwapchainCreateInfoKHR swapInfo = {};
    swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapInfo.clipped = VK_TRUE;
    swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapInfo.imageArrayLayers = 1;
    swapInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapInfo.imageExtent = extent;
    swapInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
    swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapInfo.minImageCount = surfaceCaps.minImageCount;
    swapInfo.queueFamilyIndexCount = 0;
    swapInfo.surface = surface;
    swapInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapInfo.preTransform = surfaceCaps.currentTransform;
    vkfunc(result, "Loi: tao swapchain khong thanh cong!",
        vkCreateSwapchainKHR(device, &swapInfo, nullptr, &swapchain)
    );
    std::cout << "Tao swapchain thanh cong.\n";
}
