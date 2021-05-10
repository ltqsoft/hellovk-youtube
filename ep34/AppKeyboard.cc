#include "Application.hh"


void Application::keyboard()
{
    // nhấn phím W: viewer tiến về phía trước
    if(glfwGetKey(wnd, GLFW_KEY_W) == GLFW_PRESS) {
        viewer.pos += viewer.front * viewer.moveSpeed;
    }
    // nhấn phím S: viewer lùi lại
    else if(glfwGetKey(wnd, GLFW_KEY_S) == GLFW_PRESS) {
        viewer.pos -= viewer.front * viewer.moveSpeed;
    }

    // nhấn phím A: viewer dịch sang trái
    if(glfwGetKey(wnd, GLFW_KEY_A) == GLFW_PRESS) {
        viewer.pos -= viewer.right * viewer.moveSpeed;
    }
    // nhấn phím D: viewer dịch sang phải
    else if(glfwGetKey(wnd, GLFW_KEY_D) == GLFW_PRESS) {
        viewer.pos += viewer.right * viewer.moveSpeed;
    }

    // nhấn phím ESC: thoát chương trình
    if(glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(wnd, GLFW_TRUE);
    }
}
