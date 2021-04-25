#include "Application.hh"
#include <vector>


Application::Application()
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
    GLFWwindow* wnd = glfwCreateWindow(
        1024, 768, "Hello, jackass!", nullptr, nullptr
    );
    if(wnd == nullptr) throw rterr(
        ERR_GLFW_WINDOW, "Loi: Tao cua so Vulkan khong thanh cong!"
    );
    std::cout << "Tao cua so Vulkan thanh cong.\n";

    this->createInstance();

    while(!glfwWindowShouldClose(wnd))
    {
        glfwPollEvents();
    }

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(wnd);
    glfwTerminate();
    std::cout << "Chuong trinh ket thuc.\n";
}


Application::~Application()
{}


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
    