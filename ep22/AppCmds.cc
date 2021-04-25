#include "Application.hh"

void Application::createCmds()
{
    VkCommandPoolCreateInfo cpInfo = {};
    cpInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cpInfo.queueFamilyIndex = queueFamily;
    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao cmd pool khong thanh cong!",
        vkCreateCommandPool(device, &cpInfo, nullptr, &cmdPool)
    );

    cmds.resize(minImageCount);
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = cmdPool;
    allocInfo.commandBufferCount = static_cast<uint32_t>(cmds.size());
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    vkfunc(result, "Loi: cap phat bo nho cmd khong thanh cong!",
        vkAllocateCommandBuffers(device, &allocInfo, cmds.data())
    );

    for(uint32_t i=0; i<minImageCount; i++)
    {
        VkCommandBuffer cmd = cmds[i];
        VkCommandBufferBeginInfo cbBegInfo = {};
        cbBegInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cbBegInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        vkfunc(result, "Loi: khong the bat dau ghi cmd!",
            vkBeginCommandBuffer(cmd, &cbBegInfo);
        );

        VkRect2D renderArea = {};
        renderArea.offset.x = 0;
        renderArea.offset.y = 0;
        renderArea.extent.width = wndWidth;
        renderArea.extent.height = wndHeight;

        //VkClearValue mau_xanh_duong = {};
        //VkClearValue mau_vang = {};
        //mau_xanh_duong.color = { 0.0f, 0.0f, 1.0f, 1.0f };
        //mau_vang.color = { 1.0f, 1.0f, 0.0f, 1.0f };
        VkClearValue mau_den = {};

        VkRenderPassBeginInfo rpBegInfo = {};
        rpBegInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegInfo.renderArea = renderArea;
        rpBegInfo.renderPass = renderPass;
        rpBegInfo.clearValueCount = 1;
        rpBegInfo.pClearValues = &mau_den;
        rpBegInfo.framebuffer = framebuffers[i];
        vkCmdBeginRenderPass(cmd, &rpBegInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout, 0, 1, &descriptorSet, 0, nullptr
        );
        vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(pushData), &pushData
        );
        vkCmdDraw(cmd, 6, 1, 0, 0);
        vkCmdEndRenderPass(cmd);

        vkfunc(result, "Loi: khong the ket thuc ghi cmd!",
            vkEndCommandBuffer(cmd)
        );

        std::cout << "Tao cmd thanh cong.\n";
    }
}


void Application::createSyncObjects()
{
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao fence cho swapchain khong thanh cong!",
        vkCreateFence(device, &fenceInfo, nullptr, &imageReadyFence)
    );
    vkfunc(result, "Loi: tao fence cho queue khong thanh cong!",
        vkCreateFence(device, &fenceInfo, nullptr, &renderDoneFence)
    );

    std::cout << "Tao fence thanh cong.\n";
}



