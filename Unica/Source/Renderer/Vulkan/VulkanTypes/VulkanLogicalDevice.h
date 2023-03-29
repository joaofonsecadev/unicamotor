#pragma once

#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanLogicalDevice : public VulkanTypeInterface<VkDevice>
{
public:
    VulkanLogicalDevice(VulkanInterface* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    ~VulkanLogicalDevice() override = default;

    void Init() override;
    void Destroy() override;

    VkQueue GetVulkanGraphicsQueue() const { return m_VulkanGraphicsQueue; }
    VkQueue GetVulkanPresentImagesQueue() const { return m_VulkanPresentImagesQueue; }
private:
    VkQueue m_VulkanGraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_VulkanPresentImagesQueue = VK_NULL_HANDLE;
};
