// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include "Renderer/Vulkan/VulkanTypeInterface.h"
#include "UnicaMinimal.h"

class VulkanPhysicalDevice : public VulkanTypeInterface<VkPhysicalDevice>
{
public:
    VulkanPhysicalDevice(VulkanAPI* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override { }

    ~VulkanPhysicalDevice() override = default;

private:
    uint32 RateVulkanPhysicalDevice(const VkPhysicalDevice& VulkanPhysicalDevice) const;
    bool DeviceHasRequiredExtensions(const VkPhysicalDevice& VulkanPhysicalDevice) const;

};