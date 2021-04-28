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
}
