// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "UnicaMinimal.h"
#include "VulkanRenderWindow.h"
#include "VulkanSwapChainSupportDetails.h"
#include "Renderer/RenderInterface.h"
#include "Renderer/Vulkan/VulkanTypes/VulkanInstance.h"
#include "Renderer/Vulkan/VulkanTypes/VulkanPhysicalDevice.h"
#include "Renderer/Vulkan/VulkanTypes/VulkanWindowSurface.h"
#include "Renderer/Vulkan/VulkanTypes/VulkanLogicalDevice.h"
#include "VulkanTypes/VulkanCommandBuffer.h"
#include "VulkanTypes/VulkanCommandPool.h"
#include "VulkanTypes/VulkanFramebuffer.h"
#include "VulkanTypes/VulkanImageView.h"
#include "VulkanTypes/VulkanPipeline.h"
#include "VulkanTypes/VulkanRenderPass.h"
#include "VulkanTypes/VulkanSwapChain.h"

class RenderManager;
class VulkanInstance;
class VulkanQueueFamilyIndices;

class VulkanInterface : public RenderInterface
{
public:
	void Init() override;
	void Tick() override;
	void Shutdown() override;

	SdlRenderWindow* GetSdlRenderWindow() const { return m_SdlRenderWindow.get(); }
	VulkanInstance* GetVulkanInstance() const { return m_VulkanInstance.get(); }
	VulkanWindowSurface* GetVulkanWindowSurface() const { return m_VulkanWindowSurface.get(); }
	VulkanPhysicalDevice* GetVulkanPhysicalDevice() const { return m_VulkanPhysicalDevice.get(); }
	VulkanLogicalDevice* GetVulkanLogicalDevice() const { return m_VulkanLogicalDevice.get(); }
	VulkanSwapChain* GetVulkanSwapChain() const { return m_VulkanSwapChain.get(); }
	VulkanRenderPass* GetVulkanRenderPass() const { return m_VulkanRenderPass.get(); }
	VulkanPipeline* GetVulkanPipeline() const { return m_VulkanPipeline.get(); }
	VulkanCommandPool* GetVulkanCommandPool() const { return m_VulkanCommandPool.get(); }
	
	std::vector<std::unique_ptr<VulkanFramebuffer>>& GetVulkanFramebuffers() { return m_VulkanFramebuffers; }

	bool GetValidationLayersEnabled() const { return m_bValidationLayersEnabled; }
	const std::vector<const char*>& GetRequestedValidationLayers() const { return m_RequestedValidationLayers; }
	const std::vector<const char*>& GetRequiredDeviceExtensions() const { return m_RequiredDeviceExtensions; }

	uint8 GetMaxFramesInFlight() const { return m_MaxFramesInFlight; }
	VulkanQueueFamilyIndices GetDeviceQueueFamilies(const VkPhysicalDevice& VulkanPhysicalDevice);
	VulkanSwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& VulkanPhysicalDevice);

private:
	void DrawFrame();
	
	void InitVulkanImageViews();
	void InitVulkanFramebuffers();
	void InitSyncObjects();

	void DestroySyncObjects();

	std::unique_ptr<SdlRenderWindow> m_SdlRenderWindow = std::make_unique<SdlRenderWindow>();

	std::unique_ptr<VulkanInstance> m_VulkanInstance = std::make_unique<VulkanInstance>(this);
	std::unique_ptr<VulkanWindowSurface> m_VulkanWindowSurface = std::make_unique<VulkanWindowSurface>(this);
	std::unique_ptr<VulkanPhysicalDevice> m_VulkanPhysicalDevice = std::make_unique<VulkanPhysicalDevice>(this);
	std::unique_ptr<VulkanLogicalDevice> m_VulkanLogicalDevice = std::make_unique<VulkanLogicalDevice>(this);
	std::unique_ptr<VulkanSwapChain> m_VulkanSwapChain = std::make_unique<VulkanSwapChain>(this);
	std::unique_ptr<VulkanRenderPass> m_VulkanRenderPass = std::make_unique<VulkanRenderPass>(this);
	std::unique_ptr<VulkanPipeline> m_VulkanPipeline = std::make_unique<VulkanPipeline>(this);
	std::unique_ptr<VulkanCommandPool> m_VulkanCommandPool = std::make_unique<VulkanCommandPool>(this);
	std::unique_ptr<VulkanCommandBuffer> m_VulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>(this);

	std::vector<std::unique_ptr<VulkanImageView>> m_VulkanImageViews;
	std::vector<std::unique_ptr<VulkanFramebuffer>> m_VulkanFramebuffers;

	std::vector<VkSemaphore> m_SemaphoresImageAvailable;
	std::vector<VkSemaphore> m_SemaphoresRenderFinished;
	std::vector<VkFence> m_FencesInFlight;
	
	const std::vector<const char*> m_RequiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const std::vector<const char*> m_RequestedValidationLayers = { "VK_LAYER_KHRONOS_validation" };

	const uint8 m_MaxFramesInFlight = 2;
	uint8 m_CurrentFrameIndex = 0;

private:
#if UNICA_SHIPPING
	const bool m_bValidationLayersEnabled = false;
#else
	const bool m_bValidationLayersEnabled = true;
#endif
};
