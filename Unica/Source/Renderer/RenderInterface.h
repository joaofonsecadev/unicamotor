// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

private:
    void CreateVulkanInstance();
    void CreateVulkanDebugMessenger();
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

    VkInstance m_VulkanInstance;
    VkDebugUtilsMessengerEXT m_VulkanDebugMessenger;
};
