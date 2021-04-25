#include "Application.hh"


void Application::createSwapchain()
{
    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: tao surface that bai!",
        glfwCreateWindowSurface(instance, wnd, nullptr, &surface);
    );

    VkExtent2D extent;
    extent.width = wndWidth;
    extent.height = wndHeight;

    VkSurfaceCapabilitiesKHR surfaceCaps;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        physicalDevice, surface, &surfaceCaps
    );
    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        physicalDevice, queueFamily, surface, &presentSupport
    );
    if(!presentSupport) throw rterr(
        ERR_VK_PRESENT_SUPPORT,
        "Loi: surface khong co tinh nang present cho queue family!"
    );

    VkSwapchainCreateInfoKHR swapInfo = {};
    swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapInfo.clipped = VK_TRUE;
    swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapInfo.imageArrayLayers = 1;
    swapInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapInfo.imageExtent = extent;
    swapInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
    swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapInfo.minImageCount = surfaceCaps.minImageCount;
    swapInfo.queueFamilyIndexCount = 0;
    swapInfo.surface = surface;
    swapInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapInfo.preTransform = surfaceCaps.currentTransform;
    vkfunc(result, "Loi: tao swapchain khong thanh cong!",
        vkCreateSwapchainKHR(device, &swapInfo, nullptr, &swapchain)
    );
    std::cout << "Tao swapchain thanh cong.\n";
}


void Application::createSwapchainViews()
{
    uint32_t count = 0;
    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi: khong the lay swapchain image!",
        vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr)
    )
    swapchainImages.resize(count);
    vkGetSwapchainImagesKHR(
        device, swapchain, &count, swapchainImages.data());

    swapchainViews.resize(count);
    for(size_t i=0; i<count; ++i)
    {
        swapchainViews[i] = this->createImageView(
            swapchainImages[i], VK_FORMAT_B8G8R8A8_UNORM,
            VK_IMAGE_ASPECT_COLOR_BIT
        );
    }

    std::cout << "Cai dat swapchain view thanh cong.\n";
}


VkImageView Application::createImageView(
    VkImage image, VkFormat format, VkImageAspectFlags aspect
)
{
    VkImageView kq = VK_NULL_HANDLE;

    VkImageViewCreateInfo ivInfo = {};
    ivInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ivInfo.image = image;
    ivInfo.format = format;
    ivInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    ivInfo.subresourceRange.aspectMask = aspect;
    ivInfo.subresourceRange.layerCount = 1;
    ivInfo.subresourceRange.levelCount = 1;
    VkResult result = VK_ERROR_UNKNOWN;
    vkfunc(result, "Loi, tao image view khong thanh cong!",
        vkCreateImageView(device, &ivInfo, nullptr, &kq)
    );

    std::cout << "Tao image view thanh cong.\n";
    return kq;
}
