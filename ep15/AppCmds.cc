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

        VkClearValue mau_xanh_duong = {};
        VkClearValue mau_vang = {};
        mau_xanh_duong.color = { 0.0f, 0.0f, 1.0f, 1.0f };
        mau_vang.color = { 1.0f, 1.0f, 0.0f, 1.0f };

        VkRenderPassBeginInfo rpBegInfo = {};
        rpBegInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegInfo.renderArea = renderArea;
        rpBegInfo.renderPass = renderPass;
        rpBegInfo.clearValueCount = 1;
        rpBegInfo.pClearValues = &mau_vang;
        rpBegInfo.framebuffer = framebuffers[i];
        vkCmdBeginRenderPass(cmd, &rpBegInfo, VK_SUBPASS_CONTENTS_INLINE);
        // bla bla..
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


void Application::draw()
{
    VkResult result = VK_ERROR_UNKNOWN;
    uint32_t nextImageIdx = 0;
    
    vkfunc(result, "Loi: khong the yeu cau swapchain image tiep theo!",
        vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
            VK_NULL_HANDLE, imageReadyFence, &nextImageIdx)
    );
    vkWaitForFences(device, 1, &imageReadyFence, VK_TRUE, UINT64_MAX);

    VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmds[nextImageIdx];
    submitInfo.pWaitDstStageMask = &waitStages;
    vkfunc(result, "Loi: khong the day cmd vao queue!",
        vkQueueSubmit(queue, 1, &submitInfo, renderDoneFence)
    );
    vkWaitForFences(device, 1, &renderDoneFence, VK_TRUE, UINT64_MAX);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &nextImageIdx;
    vkfunc(result, "Loi: khong the trinh dien image len cua so!",
        vkQueuePresentKHR(queue, &presentInfo)
    );

    VkFence allFences[2] = { imageReadyFence, renderDoneFence };
    vkResetFences(device, 2, allFences);
}
