#include "Application.hh"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


static const char* const TEXTURE_FILE_PATH = "star_butterfly.jpg";


void Application::createTexture()
{
    int width = 512, height = 512, channels = 4;
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

    stbi_uc* pixels = stbi_load(TEXTURE_FILE_PATH, &width, &height, &channels, STBI_rgb_alpha);
    if(pixels == nullptr) throw rterr(ERR_TEXTURE_FILE_LOAD, "Loi: khong the doc file hinh anh!");
    Buffer temp = this->createTempBuffer(width * height * 4, pixels);
    stbi_image_free(pixels);

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

    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    texture.view = this->createImageView(texture.id, format, aspect);

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

    this->beginCopy();

    VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = texture.id;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.oldLayout = imgInfo.initialLayout;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = 
        VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = aspect;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;
    
    vkCmdPipelineBarrier(copyCmd, srcStage, dstStage, 0,
        0, nullptr, 0, nullptr,
        1, &barrier
    );

    VkBufferImageCopy region = {};
    region.imageExtent.width = width;
    region.imageExtent.height = height;
    region.imageExtent.depth = 1;
    region.imageSubresource.aspectMask = aspect;
    region.imageSubresource.layerCount = 1;

    vkCmdCopyBufferToImage(copyCmd, temp.id, texture.id, barrier.newLayout,
        1, &region
    );

    srcStage = dstStage;
    dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    barrier.srcAccessMask = barrier.dstAccessMask;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout = barrier.newLayout;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    vkCmdPipelineBarrier(copyCmd, srcStage, dstStage, 0,
        0, nullptr, 0, nullptr,
        1, &barrier
    );

    this->endCopy();

    vkDestroyBuffer(device, temp.id, nullptr);
    vkFreeMemory(device, temp.mem, nullptr);

    std::cout << "Tao texture thanh cong.\n";
}
