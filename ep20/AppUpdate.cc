#include "Application.hh"
#include <cstring>


void Application::update()
{
    projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f);

    glm::vec3 trianglePosition = glm::vec3(-2.0f, -2.0f, 0.0f);
    float gocQuay = glm::radians(45.0f) * (float)glfwGetTime();
    glm::vec3 trucZ = glm::vec3(0.0f, 0.0f, 1.0f);
    float kichThuoc = glm::sin((float)glfwGetTime());

    projectionMatrix = glm::translate(projectionMatrix, trianglePosition);
    projectionMatrix = glm::rotate(projectionMatrix, gocQuay, trucZ);
    projectionMatrix = glm::scale(projectionMatrix, glm::vec3(kichThuoc));

    memcpy(ubo.mapData, &projectionMatrix, sizeof(projectionMatrix));
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
