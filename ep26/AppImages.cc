#include "Application.hh"


void Application::createTexture()
{
    uint32_t width = 512, height = 512;
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

    VkImageCreateInfo imgInfo = {};
    imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imgInfo.imageType = VK_IMAGE_TYPE_2D;
    imgInfo.arrayLayers = 1;
    imgInfo.extent.width = width;
    imgInfo.extent.height = height;
    imgInfo.extent.depth = 1;
    imgInfo.format = format;
    imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imgInfo.mipLevels = 1;
    imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imgInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imgInfo.usage = 
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao image khong thanh cong!",
        vkCreateImage(device, &imgInfo, nullptr, &texture.id)
    );

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(device, texture.id, &memReqs);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = this->getMemType(
        memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    vkfunc(result, "Loi: cap phat bo nho cho image khong thanh cong!",
        vkAllocateMemory(device, &allocInfo, nullptr, &texture.mem)
    );

    vkfunc(result, "Loi: khong the lien ket image voi memory!",
        vkBindImageMemory(device, texture.id, texture.mem, 0);
    )

    texture.view = this->createImageView(texture.id, format, VK_IMAGE_ASPECT_COLOR_BIT);

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = samplerInfo.addressModeV = samplerInfo.addressModeW = 
        VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
    
    vkfunc(result, "Loi: tao sampler khong thanh cong!",
        vkCreateSampler(device, &samplerInfo, nullptr, &texture.sampler)
    );

    std::cout << "Tao texture thanh cong, nhung chua co du lieu.\n";
}
