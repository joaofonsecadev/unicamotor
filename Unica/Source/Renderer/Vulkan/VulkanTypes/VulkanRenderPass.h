#pragma once
#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanRenderPass : public VulkanTypeInterface<VkRenderPass>
{
public:
    VulkanRenderPass(VulkanAPI* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;
    
    ~VulkanRenderPass() override = default;
};
