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
        mau_xanh_duong.color = { 0.0f, 0.0f, 1.0f, 1.0f };

        VkRenderPassBeginInfo rpBegInfo = {};
        rpBegInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBegInfo.renderArea = renderArea;
        rpBegInfo.renderPass = renderPass;
        rpBegInfo.clearValueCount = 1;
        rpBegInfo.pClearValues = &mau_xanh_duong;
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
