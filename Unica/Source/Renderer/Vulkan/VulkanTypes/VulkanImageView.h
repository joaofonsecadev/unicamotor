#pragma once

#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanImageView : public VulkanTypeInterface<VkImageView>
{
public:
    VulkanImageView(VulkanInterface* OwningVulkanAPI, VkImage VulkanSwapChainImage) : VulkanTypeInterface(OwningVulkanAPI), m_VulkanSwapChainImage(VulkanSwapChainImage) { }
    
    void Init() override;
    void Destroy() override;
    
    ~VulkanImageView() override = default;

private:
    VkImage m_VulkanSwapChainImage = VK_NULL_HANDLE;
};