#ifndef DATA_HH
#define DATA_HH

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstring>

#define SO_LUONG_INSTANCE 4 // vẽ 4 hình vuông ABCD.


struct Buffer {
    VkBuffer id = VK_NULL_HANDLE;
    VkDeviceMemory mem = VK_NULL_HANDLE;
    void* mapData = nullptr;
};


struct UniformBufferData {
    glm::mat4 viewMatrix;
    glm::mat4 worldMatrices[SO_LUONG_INSTANCE];
};


struct PushConstData {
    glm::vec3 lightColor;
};


struct Image {
    VkImage id;
    VkDeviceMemory mem;
    VkImageView view;
    VkSampler sampler = VK_NULL_HANDLE;
};


struct Vertex {
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 textureCoord;
};


struct Mesh {
    Buffer vertexBuffer;
    Buffer indexBuffer;
    Image texture;
    VkDescriptorSet descriptorSet;
    uint32_t indexCount;
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
