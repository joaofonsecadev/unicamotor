// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "UnicaMinimal.h"
#include "VulkanSwapChainSupportDetails.h"

class RenderManager;
class VulkanQueueFamilyIndices;

class VulkanAPI
{
public:
    VulkanAPI(const RenderManager* OwningRenderManager);
    ~VulkanAPI();

private:
    void CreateVulkanInstance();
	void AddRequiredExtensions(VkInstanceCreateInfo& VulkanCreateInfo, std::vector<const char*>& RequiredExtensions);
	void AddValidationLayers(VkInstanceCreateInfo& VulkanCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& VulkanDebugCreateInfo);
	
	void CreateVulkanWindowSurface();
	
	void SelectVulkanPhysicalDevice();
	uint32 RateVulkanPhysicalDevice(const VkPhysicalDevice& VulkanPhysicalDevice);
	VulkanQueueFamilyIndices GetDeviceQueueFamilies(const VkPhysicalDevice& VulkanPhysicalDevice);
	bool DeviceHasRequiredExtensions(const VkPhysicalDevice& VulkanPhysicalDevice);
	
	void CreateVulkanLogicalDevice();

	VulkanSwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& VulkanPhysicalDevice);
	VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableSurfaceFormats);
	VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);
	VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& SurfaceCapabilities);
	void CreateSwapChain();

	void CreateImageViews();

	void CreateGraphicsPipeline();
	
	void CreateVulkanDebugMessenger();
	void PopulateVulkanDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& VulkanCreateInfo);
    
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
	VkQueue m_VulkanGraphicsQueue = VK_NULL_HANDLE;
	VkQueue m_VulkanPresentImagesQueue = VK_NULL_HANDLE;
	VkDevice m_VulkanLogicalDevice = VK_NULL_HANDLE;
	VkPhysicalDevice m_VulkanPhysicalDevice = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_VulkanDebugMessenger = VK_NULL_HANDLE;
	VkSurfaceKHR m_VulkanWindowSurface = VK_NULL_HANDLE;
	
	VkSwapchainKHR m_VulkanSwapChain = VK_NULL_HANDLE;
	VkFormat m_VulkanSwapChainImageFormat;
	VkExtent2D m_VulkanSwapChainExtent;
	std::vector<VkImage> m_VulkanSwapChainImages;
	std::vector<VkImageView> m_VulkanSwapChainImageViews;

	const RenderManager* m_OwningRenderManager = nullptr;
};
