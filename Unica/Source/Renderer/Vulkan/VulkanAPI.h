// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "UnicaMinimal.h"
#include "VulkanInstance.h"
#include "VulkanRenderWindow.h"
#include "VulkanSwapChainSupportDetails.h"
#include "VulkanWindowSurface.h"
#include "Renderer/RenderInterface.h"

class RenderManager;
class VulkanInstance;
class VulkanQueueFamilyIndices;

class VulkanAPI : public RenderInterface
{
public:
	void Init() override;
	void Tick() override;
	void Shutdown() override;

	GlfwRenderWindow* GetGlfwRenderWindow() const { return m_GlfwRenderWindow.get(); }
	VulkanInstance* GetVulkanInstance() const { return m_VulkanInstance.get(); }

	bool GetValidationLayersEnabled() const { return m_bValidationLayersEnabled; }
	const std::vector<const char*>& GetRequestedValidationLayers() const { return m_RequestedValidationLayers; }

private:
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

	void CreateRenderPass();

	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(const std::vector<char>& ShaderBinary);

	void CreateFramebuffers();

	void CreateCommandPool();

	std::unique_ptr<GlfwRenderWindow> m_GlfwRenderWindow = std::make_unique<GlfwRenderWindow>();

	std::unique_ptr<VulkanInstance> m_VulkanInstance = std::make_unique<VulkanInstance>(this);
	std::unique_ptr<VulkanWindowSurface> m_VulkanWindowSurface = std::make_unique<VulkanWindowSurface>(this);


	VkQueue m_VulkanGraphicsQueue = VK_NULL_HANDLE;
	VkQueue m_VulkanPresentImagesQueue = VK_NULL_HANDLE;
	VkDevice m_VulkanLogicalDevice = VK_NULL_HANDLE;
	VkPhysicalDevice m_VulkanPhysicalDevice = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_VulkanDebugMessenger = VK_NULL_HANDLE;
	
	VkSwapchainKHR m_VulkanSwapChain = VK_NULL_HANDLE;
	VkFormat m_VulkanSwapChainImageFormat;
	VkExtent2D m_VulkanSwapChainExtent;
	std::vector<VkImage> m_VulkanSwapChainImages;
	std::vector<VkImageView> m_VulkanSwapChainImageViews;

	VkRenderPass m_VulkanRenderPass;
	VkPipelineLayout m_VulkanPipelineLayout;
	VkPipeline m_VulkanGraphicsPipeline;

	std::vector<VkFramebuffer> m_SwapChainFramebuffers;

	VkCommandPool m_VulkanCommandPool;

	const std::vector<const char*> m_RequiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const std::vector<const char*> m_RequestedValidationLayers = { "VK_LAYER_KHRONOS_validation" };

private:
#if UNICA_SHIPPING
	const bool m_bValidationLayersEnabled = false;
#else
	const bool m_bValidationLayersEnabled = true;
#endif
};
