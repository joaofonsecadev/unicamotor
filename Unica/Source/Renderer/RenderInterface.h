// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "UnicaMinimal.h"

class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

// VulkanAPI 
private:
	void InitVulkanInterface();
	void DestroyVulkanInterface();

    void CreateVulkanInstance();
    void CreateVulkanDebugMessenger();
	void SelectVulkanPhysicalDevice();

	uint32 RateVulkanPhysicalDevice(const VkPhysicalDevice& VulkanPhysicalDevice);
	
    void PopulateVulkanDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& VulkanCreateInfo);
    void AddRequiredExtensions(VkInstanceCreateInfo& VulkanCreateInfo, std::vector<const char*>& RequiredExtensions);
    void AddValidationLayers(VkInstanceCreateInfo& VulkanCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& VulkanDebugCreateInfo);
	VkResult CreateVulkanDebugUtilsMessenger(VkInstance VulkanInstance,
		const VkDebugUtilsMessengerCreateInfoEXT* VulkanCreateInfo,
		const VkAllocationCallbacks* VulkanAllocator,
		VkDebugUtilsMessengerEXT* VulkanDebugMessenger
	);
    void DestroyVulkanDebugUtilsMessengerEXT(
        VkInstance VulkanInstance,
        VkDebugUtilsMessengerEXT VulkanDebugMessenger,
        const VkAllocationCallbacks* VulkanAllocator
    );
    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT MessageType,
        const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
        void* UserData
    );

    VkInstance m_VulkanInstance = VK_NULL_HANDLE;
	VkPhysicalDevice m_VulkanPhysicalDevice = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_VulkanDebugMessenger = VK_NULL_HANDLE;
};
