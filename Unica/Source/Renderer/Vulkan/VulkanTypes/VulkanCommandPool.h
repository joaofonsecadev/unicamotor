#pragma once
#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanCommandPool : public VulkanTypeInterface<VkCommandPool>
{
public:
    VulkanCommandPool(VulkanInterface* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;
    
    ~VulkanCommandPool() override = default;
};
