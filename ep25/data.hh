#ifndef DATA_HH
#define DATA_HH

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Buffer {
    VkBuffer id;
    VkDeviceMemory mem;
    void* mapData;
};


struct UniformBufferData {
    glm::mat4 viewMatrix;
    glm::mat4 worldMatrix;
};


struct PushConstData {
    glm::vec3 lightColor;
};


struct Viewer {
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float moveSpeed;
    float turnSpeed;
};


#endif
