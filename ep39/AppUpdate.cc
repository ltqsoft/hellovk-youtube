#include "Application.hh"


void Application::update()
{
    //projMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f);
    projMatrix = glm::perspective(
        glm::radians(45.0f), (float)wndWidth / wndHeight, 0.1f, 125.0f
    );
    projMatrix[1][1] = -projMatrix[1][1];

    // vị trí 4 hình vuông
    glm::vec3 positions[SO_LUONG_INSTANCE];
    positions[0] = glm::vec3(0.0f, 2.0f, 0.0f);
    positions[1] = positions[0] + glm::vec3(0.0f, 0.0f, 4.0f);
    positions[2] = positions[0] + glm::vec3(0.0f, 0.0f, -4.0f);
    positions[3] = positions[0] + glm::vec3(4.0f, 0.0f, 0.0f);

    // góc quay 4 hình vuông
    float gocQuay[SO_LUONG_INSTANCE];
    gocQuay[0] = gocQuay[2] = glm::radians(45.0f) * (float)glfwGetTime();
    gocQuay[1] = gocQuay[3] = -gocQuay[0];

    glm::vec3 trucZ = glm::vec3(0.0f, 0.0f, 1.0f);
    float kichThuoc = 1.5f;

    ubo2Data.viewMatrix = glm::lookAt(viewer.pos, viewer.pos + viewer.front, viewer.up);

    for(uint32_t i=0; i < SO_LUONG_INSTANCE; i++)
    {
        ubo2Data.worldMatrices[i] = glm::mat4(1.0f);
        ubo2Data.worldMatrices[i] = glm::translate(ubo2Data.worldMatrices[i], positions[i]);
        ubo2Data.worldMatrices[i] = glm::rotate(ubo2Data.worldMatrices[i], gocQuay[i], trucZ);
        ubo2Data.worldMatrices[i] = glm::scale(ubo2Data.worldMatrices[i], glm::vec3(kichThuoc));
    }

    memcpy(ubo1.mapData, &projMatrix, sizeof(projMatrix));
    memcpy(ubo2.mapData, &ubo2Data, sizeof(ubo2Data));
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
