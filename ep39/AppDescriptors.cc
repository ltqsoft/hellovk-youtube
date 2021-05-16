#include "Application.hh"


void Application::createDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(SO_LUONG_BINDING, {});

    bindings[BIND_ID_PROJECTION_MATRIX].binding = BIND_ID_PROJECTION_MATRIX;
    bindings[BIND_ID_PROJECTION_MATRIX].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[BIND_ID_PROJECTION_MATRIX].descriptorCount = 1;
    bindings[BIND_ID_PROJECTION_MATRIX].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    bindings[BIND_ID_UBO2].binding = BIND_ID_UBO2;
    bindings[BIND_ID_UBO2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[BIND_ID_UBO2].descriptorCount = 1;
    bindings[BIND_ID_UBO2].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    bindings[BIND_ID_TEXTURE].binding = BIND_ID_TEXTURE;
    bindings[BIND_ID_TEXTURE].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[BIND_ID_TEXTURE].descriptorCount = 1;
    bindings[BIND_ID_TEXTURE].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo dslInfo = {};
    dslInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    dslInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    dslInfo.pBindings = bindings.data();

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao descriptor set layout khong thanh cong!",
        vkCreateDescriptorSetLayout(device, &dslInfo, nullptr, &dsLayout)
    );

    std::cout << "Tao descriptor set layout thanh cong.\n";
}


void Application::createDescriptorPool()
{
    const uint32_t SO_LUONG_DESCRIPTOR_SET = 16;

    std::vector<VkDescriptorPoolSize> poolSizes;
    poolSizes.resize(2);
    poolSizes[0].descriptorCount = SO_LUONG_DESCRIPTOR_SET;
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[1].descriptorCount = SO_LUONG_DESCRIPTOR_SET;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    VkDescriptorPoolCreateInfo dpInfo = {};
    dpInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    dpInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    dpInfo.pPoolSizes = poolSizes.data();
    dpInfo.maxSets = SO_LUONG_DESCRIPTOR_SET;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao descriptor pool khong thanh cong!",
        vkCreateDescriptorPool(device, &dpInfo, nullptr, &descriptorPool)
    );

    std::cout << "Tao descriptor pool thanh cong.\n";
}
