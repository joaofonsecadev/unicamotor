#include "VulkanLogicalDevice.h"

#include <set>

#include "Log/Logger.h"
#include "Renderer/Vulkan/VulkanQueueFamilyIndices.h"
#include "Renderer/Vulkan/VulkanInterface.h"

void VulkanLogicalDevice::Init()
{
    VulkanQueueFamilyIndices QueueFamilyIndices = m_OwningVulkanAPI->GetDeviceQueueFamilies(m_OwningVulkanAPI->GetVulkanPhysicalDevice()->GetVulkanObject());
	if (!QueueFamilyIndices.WasSet())
	{
		UNICA_LOG(spdlog::level::critical, "No support for graphics and image presentation queues");
	}

	std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;
	std::set<uint32> UniqueQueueFamilies = { QueueFamilyIndices.GetGraphicsFamily().value(), QueueFamilyIndices.GetPresentImagesFamily().value() };
	
	const float QueuePriority = 1.f;
	for (const uint32 UniqueQueueFamily : UniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo QueueCreateInfo { };
		QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfo.queueFamilyIndex = UniqueQueueFamily;
		QueueCreateInfo.queueCount = 1;
		QueueCreateInfo.pQueuePriorities = &QueuePriority;
		QueueCreateInfos.push_back(QueueCreateInfo);
	}

	VkPhysicalDeviceFeatures DeviceFeatures { };

	VkDeviceCreateInfo DeviceCreateInfo { };
	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceCreateInfo.queueCreateInfoCount = static_cast<uint32>(QueueCreateInfos.size());
	DeviceCreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
	DeviceCreateInfo.pEnabledFeatures = &DeviceFeatures;

	DeviceCreateInfo.enabledExtensionCount = static_cast<uint32>(m_OwningVulkanAPI->GetRequiredDeviceExtensions().size());
	DeviceCreateInfo.ppEnabledExtensionNames = m_OwningVulkanAPI->GetRequiredDeviceExtensions().data();

	if (m_OwningVulkanAPI->GetValidationLayersEnabled())
	{
		DeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_OwningVulkanAPI->GetRequestedValidationLayers().size());
		DeviceCreateInfo.ppEnabledLayerNames = m_OwningVulkanAPI->GetRequestedValidationLayers().data();
	}
	else
	{
		DeviceCreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_OwningVulkanAPI->GetVulkanPhysicalDevice()->GetVulkanObject(), &DeviceCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Couldn't create a VulkanLogicalDevice");
	}
	
	vkGetDeviceQueue(m_VulkanObject, QueueFamilyIndices.GetGraphicsFamily().value(), 0, &m_VulkanGraphicsQueue);
	vkGetDeviceQueue(m_VulkanObject, QueueFamilyIndices.GetPresentImagesFamily().value(), 0, &m_VulkanPresentImagesQueue);
}

void VulkanLogicalDevice::Destroy()
{
    vkDestroyDevice(m_VulkanObject, nullptr);
}
