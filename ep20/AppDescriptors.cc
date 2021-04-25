#include "Application.hh"


void Application::createDescriptorSetLayout()
{
    

    VkDescriptorSetLayoutBinding binding = {};
    binding.binding = BIND_ID_PROJECTION_MATRIX;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.descriptorCount = 1;
    binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo dslInfo = {};
    dslInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    dslInfo.bindingCount = 1;
    dslInfo.pBindings = &binding;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao descriptor set layout khong thanh cong!",
        vkCreateDescriptorSetLayout(device, &dslInfo, nullptr, &dsLayout)
    );

    std::cout << "Tao descriptor set layout thanh cong.\n";
}


void Application::createDescriptorSet()
{
    VkDescriptorPoolSize poolSize = {};
    poolSize.descriptorCount = 1;
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    VkDescriptorPoolCreateInfo dpInfo = {};
    dpInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    dpInfo.poolSizeCount = 1;
    dpInfo.pPoolSizes = &poolSize;
    dpInfo.maxSets = 1;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao descriptor pool khong thanh cong!",
        vkCreateDescriptorPool(device, &dpInfo, nullptr, &descriptorPool)
    );

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &dsLayout;
    vkfunc(result, "Loi: cap phat bo nho cho descriptor set khong thanh cong!",
        vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet)
    );

    VkDescriptorBufferInfo dbufInfo = {};
    dbufInfo.buffer = ubo.id;
    dbufInfo.offset = 0;
    dbufInfo.range = sizeof(projectionMatrix);

    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.pBufferInfo = &dbufInfo;
    write.dstBinding = BIND_ID_PROJECTION_MATRIX;
    write.dstSet = descriptorSet;
    vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);

    std::cout << "Tao descriptor set thanh cong.\n";
}
