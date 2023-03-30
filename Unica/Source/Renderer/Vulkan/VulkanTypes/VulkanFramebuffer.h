#pragma once
#include "VulkanImageView.h"
#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanFramebuffer : public VulkanTypeInterface<VkFramebuffer>
{
public:
    VulkanFramebuffer(VulkanInterface* OwningVulkanAPI, VulkanImageView* VulkanImageView) : VulkanTypeInterface(OwningVulkanAPI), m_VulkanImageView(VulkanImageView) { }
    
    void Init() override;
    void Destroy() override;
    
    ~VulkanFramebuffer() override = default;

private:
    VulkanImageView* m_VulkanImageView = nullptr;
};
