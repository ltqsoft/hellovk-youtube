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


void Application::createDescriptorSet()
{
    VkDescriptorPoolSize poolSize = {};
    poolSize.descriptorCount = SO_LUONG_BINDING;
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

    std::vector<VkDescriptorBufferInfo> dbufInfos;
    dbufInfos.resize(SO_LUONG_BINDING, {});
    
    dbufInfos[BIND_ID_PROJECTION_MATRIX].buffer = ubo1.id;
    dbufInfos[BIND_ID_PROJECTION_MATRIX].range = sizeof(projMatrix);

    dbufInfos[BIND_ID_UBO2].buffer = ubo2.id;
    dbufInfos[BIND_ID_UBO2].range = sizeof(ubo2Data);

    std::vector<VkWriteDescriptorSet> writes;
    writes.resize(SO_LUONG_BINDING);
    
    writes[BIND_ID_PROJECTION_MATRIX].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[BIND_ID_PROJECTION_MATRIX].descriptorCount = 1;
    writes[BIND_ID_PROJECTION_MATRIX].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[BIND_ID_PROJECTION_MATRIX].pBufferInfo = &dbufInfos[BIND_ID_PROJECTION_MATRIX];
    writes[BIND_ID_PROJECTION_MATRIX].dstBinding = BIND_ID_PROJECTION_MATRIX;
    writes[BIND_ID_PROJECTION_MATRIX].dstSet = descriptorSet;

    writes[BIND_ID_UBO2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[BIND_ID_UBO2].descriptorCount = 1;
    writes[BIND_ID_UBO2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[BIND_ID_UBO2].pBufferInfo = &dbufInfos[BIND_ID_UBO2];
    writes[BIND_ID_UBO2].dstBinding = BIND_ID_UBO2;
    writes[BIND_ID_UBO2].dstSet = descriptorSet;
    
    vkUpdateDescriptorSets(device, 
        static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

    std::cout << "Tao descriptor set thanh cong.\n";
}
