#pragma once

#include "VulkanObjectInterface.h"
#include "UnicaMinimal.h"

class VulkanPhysicalDevice : public VulkanObjectInterface<VkPhysicalDevice>
{
public:
    VulkanPhysicalDevice(VulkanAPI* OwningVulkanAPI) : VulkanObjectInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override { }

    ~VulkanPhysicalDevice() override = default;

private:
    uint32 RateVulkanPhysicalDevice(const VkPhysicalDevice& VulkanPhysicalDevice) const;
    bool DeviceHasRequiredExtensions(const VkPhysicalDevice& VulkanPhysicalDevice) const;

};