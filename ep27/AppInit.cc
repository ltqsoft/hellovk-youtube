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
    this->createRenderPass();
    this->createFramebuffers();
    this->createSyncObjects();

    this->createUniformBuffers();
    this->createDescriptorSetLayout();
    this->createDescriptorSet();

    this->loadShaders();
    this->createPipelineLayout();
    this->createGraphicsPipeline();

    this->createCmds();

    this->setupViewer();
    this->createTexture();
}


void Application::cleanup()
{
    vkDestroyCommandPool(device, copyPool, nullptr);

    vkDestroyImageView(device, texture.view, nullptr);
    vkDestroyImage(device, texture.id, nullptr);
    vkDestroySampler(device, texture.sampler, nullptr);
    vkFreeMemory(device, texture.mem, nullptr);

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
