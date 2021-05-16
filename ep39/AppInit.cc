#include "Application.hh"


void Application::init()
{
    this->createWindow();
    this->createInstance();
    this->setupDebugger();
    this->pickPhysicalDevice();
    this->createLogicalDevice();

    this->createSwapchain();
    this->createSwapchainViews();
    this->createDepthImage();

    this->createRenderPass();
    this->createFramebuffers();
    this->createSyncObjects();
    this->createCmdPools();

    this->createUniformBuffers();

    this->createDescriptorSetLayout();
    this->createDescriptorPool();

    this->loadShaders();
    this->createPipelineLayout();
    this->createGraphicsPipeline();

    this->loadModel();
    this->createCmds();

    this->setupViewer();
    
}


void Application::cleanup()
{
    for(Mesh& m : meshes) {
        vkDestroyBuffer(device, m.vertexBuffer.id, nullptr);
        vkFreeMemory(device, m.vertexBuffer.mem, nullptr);
        vkDestroyBuffer(device, m.indexBuffer.id, nullptr);
        vkFreeMemory(device, m.indexBuffer.mem, nullptr);
        vkDestroySampler(device, m.texture.sampler, nullptr);
        vkDestroyImageView(device, m.texture.view, nullptr);
        vkDestroyImage(device, m.texture.id, nullptr);
        vkFreeMemory(device, m.texture.mem, nullptr);
    }

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, dsLayout, nullptr);

    vkDestroyBuffer(device, ubo1.id, nullptr);
    vkFreeMemory(device, ubo1.mem, nullptr);
    vkDestroyBuffer(device, ubo2.id, nullptr);
    vkFreeMemory(device, ubo2.mem, nullptr);

    vkDestroyShaderModule(device, vertMod, nullptr);
    vkDestroyShaderModule(device, fragMod, nullptr);

    vkDestroyFence(device, imageReadyFence, nullptr);
    vkDestroyFence(device, renderDoneFence, nullptr);

    vkDestroyCommandPool(device, cmdPool, nullptr);
    for(VkFramebuffer fb : framebuffers) {
        vkDestroyFramebuffer(device, fb, nullptr);
    }
    vkDestroyRenderPass(device, renderPass, nullptr);

    vkDestroyImageView(device, depthImage.view, nullptr);
    vkDestroyImage(device, depthImage.id, nullptr);
    vkFreeMemory(device, depthImage.mem, nullptr);

    for(VkImageView iv : swapchainViews) {
        vkDestroyImageView(device, iv, nullptr);
    }
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    
    vkDestroyDevice(device, nullptr);
    this->destroyDeb(instance, deb, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(wnd);
    glfwTerminate();
}
