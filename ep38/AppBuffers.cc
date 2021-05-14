#include "Application.hh"


void Application::createUniformBuffers()
{
    ubo1 = this->createBuffer(sizeof(projMatrix), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    ubo2 = this->createBuffer(sizeof(ubo2Data), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}


Buffer Application::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage)
{
    Buffer ret;

    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.size = size;
    bufInfo.usage = usage;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao buffer khong thanh cong!",
        vkCreateBuffer(device, &bufInfo, nullptr, &ret.id)
    );

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, ret.id, &memReqs);
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = this->getMemType(
        memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    vkfunc(result, "Loi: cap phat bo nho cho buffer khong thanh cong!",
        vkAllocateMemory(device, &allocInfo, nullptr, &ret.mem)
    );

    vkfunc(result, "Loi: khong the lien ket buffer voi memory!",
        vkBindBufferMemory(device, ret.id, ret.mem, 0);
    );

    vkfunc(result, "Loi: khong the anh xa GPU memory voi CPU memory",
        vkMapMemory(device, ret.mem, 0, bufInfo.size, 0, &ret.mapData)
    );

    std::cout << "Tao buffer thanh cong.\n";
    return ret;
}


uint32_t Application::getMemType(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
    for(uint32_t i=0; i<memProps.memoryTypeCount; ++i)
    {
        if(
            (typeBits & (1<<i)) &&
            (properties == (properties & memProps.memoryTypes[i].propertyFlags))
        ) return i;
    }

    throw rterr(ERR_VK_MEMORY_TYPE, "Loi: khong tim thay memory type phu hop!");
}


Buffer Application::createTempBuffer(VkDeviceSize size, void* data)
{
    Buffer ret = this->createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(ret.mapData, data, size);
    return ret;
}


void Application::createVertexBuffer()
{
    std::vector<Vertex> vertices;
    vertices.resize(4); // giờ chỉ cần 4 đỉnh A,B,C,D trong vertex buffer.

    const glm::vec2 A (-0.5f, -0.5f);
    const glm::vec2 B ( 0.5f, -0.5f);
    const glm::vec2 C ( 0.5f,  0.5f);
    const glm::vec2 D (-0.5f, 0.5f);

    vertices[0].position = A;
    vertices[1].position = B;
    vertices[2].position = C;
    vertices[3].position = D;

    const glm::vec3 mau_do               (1.0f, 0.0f, 0.0f);
    const glm::vec3 mau_tim              (1.0f, 0.0f, 1.0f);
    const glm::vec3 mau_xanh_duong       (0.0f, 0.0f, 1.0f);
    const glm::vec3 mau_xanh_la          (0.0f, 1.0f, 0.0f);
    const glm::vec3 mau_vang             (1.0f, 1.0f, 0.0f);

    vertices[0].color = mau_do;
    vertices[1].color = mau_xanh_duong;
    vertices[2].color = mau_xanh_la;
    vertices[3].color = mau_vang;

    const glm::vec2 textureCoordA (0.0f, 0.0f);
    const glm::vec2 textureCoordB (1.0f, 0.0f);
    const glm::vec2 textureCoordC (1.0f, 1.0f);
    const glm::vec2 textureCoordD (0.0f, 1.0f);

    vertices[0].textureCoord = textureCoordA;
    vertices[1].textureCoord = textureCoordB;
    vertices[2].textureCoord = textureCoordC;
    vertices[3].textureCoord = textureCoordD;

    VkDeviceSize vertexBufferSize = vertices.size() * sizeof(Vertex);
    vertexBuffer = this->createBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    memcpy(vertexBuffer.mapData, vertices.data(), vertexBufferSize);

    std::cout << "Tao vertex buffer thanh cong.\n";
}


void Application::createIndexBuffer()
{
    indexCount = 6;
    std::vector<uint32_t> indices;
    indices.resize(indexCount);

    indices[0] = 0; // A: index0
    indices[1] = 1; // B: index1
    indices[2] = 2; // C: index2

    indices[3] = 0; // A: index0
    indices[4] = 2; // C: index2
    indices[5] = 3; // D: index3

    VkDeviceSize indexBufferSize = indices.size() * sizeof(indices[0]);
    indexBuffer = this->createBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    memcpy(indexBuffer.mapData, indices.data(), indexBufferSize);

    std::cout << "Tao index buffer thanh cong.\n";
}
