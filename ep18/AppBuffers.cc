#include "Application.hh"


void Application::createUniformBuffer()
{
    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.size = sizeof(projectionMatrix);
    bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao uniform buffer khong thanh cong!",
        vkCreateBuffer(device, &bufInfo, nullptr, &ubo.id)
    );

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, ubo.id, &memReqs);
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = this->getMemType(
        memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    vkfunc(result, "Loi: cap phat bo nho cho uniform buffer khong thanh cong!",
        vkAllocateMemory(device, &allocInfo, nullptr, &ubo.mem)
    );

    vkfunc(result, "Loi: khong the lien ket uniform buffer voi memory!",
        vkBindBufferMemory(device, ubo.id, ubo.mem, 0);
    );

    vkfunc(result, "Loi: khong the anh xa GPU memory voi CPU memory",
        vkMapMemory(device, ubo.mem, 0, bufInfo.size, 0, &ubo.mapData)
    );

    std::cout << "Tao uniform buffer thanh cong.\n";
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
