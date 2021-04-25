#include "Application.hh"


void Application::createUniformBuffers()
{
    ubo1 = this->createUniformBuffer(sizeof(projMatrix));
    ubo2 = this->createUniformBuffer(sizeof(ubo2Data));
}


Buffer Application::createUniformBuffer(VkDeviceSize size)
{
    Buffer ret;

    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.size = size;
    bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao uniform buffer khong thanh cong!",
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
    vkfunc(result, "Loi: cap phat bo nho cho uniform buffer khong thanh cong!",
        vkAllocateMemory(device, &allocInfo, nullptr, &ret.mem)
    );

    vkfunc(result, "Loi: khong the lien ket uniform buffer voi memory!",
        vkBindBufferMemory(device, ret.id, ret.mem, 0);
    );

    vkfunc(result, "Loi: khong the anh xa GPU memory voi CPU memory",
        vkMapMemory(device, ret.mem, 0, bufInfo.size, 0, &ret.mapData)
    );

    std::cout << "Tao uniform buffer thanh cong.\n";
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
