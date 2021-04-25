#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <utility>

#define ERR_GLFW_INIT       1
#define ERR_GLFW_WINDOW     2

typedef std::pair<int, std::runtime_error> rterr;

int main()
{try{
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

    while(!glfwWindowShouldClose(wnd))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(wnd);
    glfwTerminate();
    return 0;
}catch(const rterr& err){
    std::cerr << err.first << " -> " << err.second.what() << '\n';
    return err.first;
}}