#include "Application.hh"


void Application::createRenderPass()
{
    const size_t SO_LUONG_ATTACHMENT = 1, ATTACH_COLOR = 0;
    std::vector<VkAttachmentDescription> attachments;
    attachments.resize(SO_LUONG_ATTACHMENT);
    attachments[ATTACH_COLOR].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[ATTACH_COLOR].finalLayout = 
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachments[ATTACH_COLOR].format = VK_FORMAT_B8G8R8A8_UNORM;
    attachments[ATTACH_COLOR].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[ATTACH_COLOR].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[ATTACH_COLOR].stencilLoadOp = 
        VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[ATTACH_COLOR].stencilStoreOp =
        VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[ATTACH_COLOR].samples = VK_SAMPLE_COUNT_1_BIT;

    VkAttachmentReference colorRef = {};
    colorRef.attachment = ATTACH_COLOR;
    colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorRef;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    VkRenderPassCreateInfo rpInfo = {};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    rpInfo.pAttachments = attachments.data();
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;

    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao render pass khong thanh cong!",
        vkCreateRenderPass(device, &rpInfo, nullptr, &renderPass)
    );
    
    std::cout << "Tao render pass thanh cong.\n";
}
