#include "Application.hh"
#include <glm/gtx/rotate_vector.hpp>


static bool first = true;
static glm::vec2 currMousePos;
static glm::vec2 prevMousePos;
static glm::vec2 mouseDelta;


Viewer Application::viewer;


void Application::setupViewer()
{
    viewer.pos = glm::vec3(0.0f, -2.0f, 1.0f);
    viewer.front = glm::vec3(0.0f, 1.0f, 0.0f);
    viewer.up = glm::vec3(0.0f, 0.0f, 1.0f);
    viewer.moveSpeed = 0.2f;
    viewer.turnSpeed = 0.005f;

    glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(wnd, [](GLFWwindow* wnd, double x, double y)
    {
        currMousePos = glm::vec2(
            static_cast<float>(x), static_cast<float>(y)
        );

        if(first) {
            prevMousePos = currMousePos;
            first = false;
            return;
        }

        mouseDelta = currMousePos - prevMousePos;
        mouseDelta *= viewer.turnSpeed;

        viewer.front = glm::rotateZ(viewer.front, -mouseDelta.x);
        viewer.front = glm::rotateX(viewer.front, -mouseDelta.y);
        viewer.front = glm::normalize(viewer.front);
        viewer.right = glm::normalize( glm::cross(viewer.front, viewer.up) );

        prevMousePos = currMousePos;
    });
}
