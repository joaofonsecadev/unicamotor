// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanPhysicalDevice.h"

#include <map>
#include <set>

#include "UnicaMinimal.h"
#include "Logging/Logger.h"
#include "Renderer/Vulkan/VulkanInterface.h"
#include "Renderer/Vulkan/VulkanQueueFamilyIndices.h"

void VulkanPhysicalDevice::Init()
{
    uint32 VulkanPhysicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(m_OwningVulkanAPI->GetVulkanInstance()->GetVulkanObject(), &VulkanPhysicalDeviceCount, nullptr);
    if (VulkanPhysicalDeviceCount < 1)
    {
        UNICA_LOG(spdlog::level::critical, "No GPUs with Vulkan support found");
        return;
    }

    std::vector<VkPhysicalDevice> VulkanPhysicalDevices(VulkanPhysicalDeviceCount);
    vkEnumeratePhysicalDevices(m_OwningVulkanAPI->GetVulkanInstance()->GetVulkanObject(), &VulkanPhysicalDeviceCount, VulkanPhysicalDevices.data());

    std::multimap<uint32, VkPhysicalDevice> VulkanPhysicalDeviceCandidates;
    for (const VkPhysicalDevice& VulkanPhysicalDevice : VulkanPhysicalDevices)
    {
        uint32 VulkanPhysicalDeviceScore = RateVulkanPhysicalDevice(VulkanPhysicalDevice);
        VulkanPhysicalDeviceCandidates.insert(std::make_pair(VulkanPhysicalDeviceScore, VulkanPhysicalDevice));
    }

    if (VulkanPhysicalDeviceCandidates.rbegin()->first > 0)
    {
        m_VulkanObject = VulkanPhysicalDeviceCandidates.rbegin()->second;
    }
    else
    {
        UNICA_LOG(spdlog::level::critical, "No suitable GPU found");
        return;
    }
    
    VkPhysicalDeviceProperties VulkanPhysicalDeviceProperties;
    vkGetPhysicalDeviceProperties(m_VulkanObject, &VulkanPhysicalDeviceProperties);
    UNICA_LOG_DEBUG("Selected VulkanPhysicalDevice '{}'", VulkanPhysicalDeviceProperties.deviceName);
}

uint32 VulkanPhysicalDevice::RateVulkanPhysicalDevice(const VkPhysicalDevice& VulkanPhysicalDevice) const
{
    if (!m_OwningVulkanAPI->GetDeviceQueueFamilies(VulkanPhysicalDevice).WasSet())
    {
        return 0;
    }

    if (!DeviceHasRequiredExtensions(VulkanPhysicalDevice))
    {
        return 0;
    }

    const VulkanSwapChainSupportDetails SwapChainSupportDetails = m_OwningVulkanAPI->QuerySwapChainSupport(VulkanPhysicalDevice);
    if (SwapChainSupportDetails.SurfaceFormats.empty() || SwapChainSupportDetails.PresentModes.empty())
    {
        return 0;
    }
	
    uint32 Score = 1;

    VkPhysicalDeviceProperties VulkanPhysicalDeviceProperties;
    VkPhysicalDeviceFeatures VulkanPhysicalDeviceFeatures;
    vkGetPhysicalDeviceProperties(VulkanPhysicalDevice, &VulkanPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(VulkanPhysicalDevice, &VulkanPhysicalDeviceFeatures);

    if (VulkanPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        Score += 1000;
    }
	
    return Score;
}

bool VulkanPhysicalDevice::DeviceHasRequiredExtensions(const VkPhysicalDevice& VulkanPhysicalDevice) const
{
    uint32_t AvailableDeviceExtensionCount;
    vkEnumerateDeviceExtensionProperties(VulkanPhysicalDevice, nullptr, &AvailableDeviceExtensionCount, nullptr);
    std::vector<VkExtensionProperties> AvailableDeviceExtensions(AvailableDeviceExtensionCount);
    vkEnumerateDeviceExtensionProperties(VulkanPhysicalDevice, nullptr, &AvailableDeviceExtensionCount, AvailableDeviceExtensions.data());

    std::set<std::string> DeviceExtensions(m_OwningVulkanAPI->GetRequiredDeviceExtensions().begin(), m_OwningVulkanAPI->GetRequiredDeviceExtensions().end());
    for (const VkExtensionProperties& DeviceExtension : AvailableDeviceExtensions)
    {
        DeviceExtensions.erase(DeviceExtension.extensionName);
    }

    return DeviceExtensions.empty();
}
