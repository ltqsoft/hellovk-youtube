#ifndef APP_HH
#define APP_HH

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <utility>

#define ERR_GLFW_INIT       1
#define ERR_GLFW_WINDOW     2
#define ERR_VK_DEB_CALLBACK 3

#define vkfunc(result, errorMsg, expr) \
    result = expr;  \
    if(result != VK_SUCCESS) throw rterr(result, errorMsg);

typedef std::pair<int, std::runtime_error> rterr;


class Application
{
    GLFWwindow* wnd = nullptr;
    
    VkInstance instance = VK_NULL_HANDLE;
    void createInstance();

    PFN_vkCreateDebugUtilsMessengerEXT createDeb = NULL;
    PFN_vkDestroyDebugUtilsMessengerEXT destroyDeb = NULL;
    PFN_vkDebugUtilsMessengerCallbackEXT debCallback;
    VkDebugUtilsMessengerEXT deb = VK_NULL_HANDLE;
    void setupDebugger();

    void cleanup();

public:
    Application();
    ~Application();
};

#endif
