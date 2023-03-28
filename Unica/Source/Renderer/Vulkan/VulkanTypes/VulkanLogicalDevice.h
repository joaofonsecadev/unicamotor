#pragma once

#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanLogicalDevice : public VulkanTypeInterface<VkDevice>
{
public:
    VulkanLogicalDevice(VulkanInterface* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;

    ~VulkanLogicalDevice() override = default;

private:
    VkQueue m_VulkanGraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_VulkanPresentImagesQueue = VK_NULL_HANDLE;
};
