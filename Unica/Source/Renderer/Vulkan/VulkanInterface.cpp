// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanInterface.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "UnicaFileUtilities.h"
#include "fmt/format.h"
#include "shaderc/shaderc.hpp"

#include "UnicaMinimal.h"
#include "VulkanQueueFamilyIndices.h"
#include "Renderer/RenderManager.h"
#include "Shaders/ShaderUtilities.h"

void VulkanInterface::Init()
{
	m_VulkanInstance->Init();
	m_VulkanWindowSurface->Init();
	m_VulkanPhysicalDevice->Init();
	m_VulkanLogicalDevice->Init();
	m_VulkanSwapChain->Init();
	InitVulkanImageViews();
	m_VulkanRenderPass->Init();
	m_VulkanPipeline->Init();
	InitVulkanFramebuffers();
	m_VulkanCommandPool->Init();
	m_VulkanCommandBuffer->Init();
	InitSyncObjects();
}

void VulkanInterface::Tick()
{
	UNICA_PROFILE_FUNCTION
	m_SdlRenderWindow->Tick();
	DrawFrame();
}

void VulkanInterface::DrawFrame()
{
	UNICA_PROFILE_FUNCTION
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkWaitForFences");
		vkWaitForFences(m_VulkanLogicalDevice->GetVulkanObject(), 1, &FenceInFlight, VK_TRUE, UINT64_MAX);
	}
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkResetFences");
		vkResetFences(m_VulkanLogicalDevice->GetVulkanObject(), 1, &FenceInFlight);
	}
	uint32 VulkanImageIndex;
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkAcquireNextImageKHR");
		vkAcquireNextImageKHR(m_VulkanLogicalDevice->GetVulkanObject(), m_VulkanSwapChain->GetVulkanObject(), UINT64_MAX, SemaphoreImageAvailable, VK_NULL_HANDLE, &VulkanImageIndex);
	}
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkResetCommandBuffer");
		vkResetCommandBuffer(m_VulkanCommandBuffer->GetVulkanObject(), 0);
	}
	m_VulkanCommandBuffer->Record(VulkanImageIndex);

	const VkSemaphore WaitSemaphores[] = { SemaphoreImageAvailable };
	const VkSemaphore SignalSemaphores[] = { SemaphoreRenderFinished };
	const VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	
	VkSubmitInfo SubmitInfo { };
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;	
	SubmitInfo.waitSemaphoreCount = 1;
	SubmitInfo.pWaitSemaphores = WaitSemaphores;
	SubmitInfo.pWaitDstStageMask = WaitStages;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = m_VulkanCommandBuffer->GetCommandBufferObject();
	SubmitInfo.signalSemaphoreCount = 1;
	SubmitInfo.pSignalSemaphores = SignalSemaphores;
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkQueueSubmit");
		if (vkQueueSubmit(m_VulkanLogicalDevice->GetVulkanGraphicsQueue(), 1, &SubmitInfo, FenceInFlight) != VK_SUCCESS)
		{
			UNICA_LOG_CRITICAL("Failed to submit draw command buffer!");
		}
	}

	VkPresentInfoKHR VulkanPresentInfo{};
	VulkanPresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	VulkanPresentInfo.waitSemaphoreCount = 1;
	VulkanPresentInfo.pWaitSemaphores = SignalSemaphores;

	const VkSwapchainKHR VulkanSwapChains[] = { m_VulkanSwapChain->GetVulkanObject() };
	VulkanPresentInfo.swapchainCount = 1;
	VulkanPresentInfo.pSwapchains = VulkanSwapChains;
	VulkanPresentInfo.pImageIndices = &VulkanImageIndex;
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkQueuePresentKHR");
		vkQueuePresentKHR(m_VulkanLogicalDevice->GetVulkanPresentImagesQueue(), &VulkanPresentInfo);
	}
}

void VulkanInterface::InitVulkanImageViews()
{
	uint32 SwapChainImageIteration = 0;
	m_VulkanImageViews.resize(m_VulkanSwapChain->GetVulkanSwapChainImages().size());
	for (const VkImage& SwapChainImage : m_VulkanSwapChain->GetVulkanSwapChainImages())
	{
		m_VulkanImageViews[SwapChainImageIteration] = std::make_unique<VulkanImageView>(this, SwapChainImage);
		m_VulkanImageViews[SwapChainImageIteration]->Init();
		SwapChainImageIteration++;
	}
}

void VulkanInterface::InitVulkanFramebuffers()
{
	uint32 SwapChainImageViewLoopIndex = 0;
	m_VulkanFramebuffers.resize(m_VulkanImageViews.size());
	for (const std::unique_ptr<VulkanImageView>& VulkanImageView : m_VulkanImageViews)
	{
		m_VulkanFramebuffers[SwapChainImageViewLoopIndex] = std::make_unique<VulkanFramebuffer>(this, VulkanImageView.get());
		m_VulkanFramebuffers[SwapChainImageViewLoopIndex]->Init();
		SwapChainImageViewLoopIndex++;
	}
}

void VulkanInterface::InitSyncObjects()
{
	VkSemaphoreCreateInfo SemaphoreInfo { };
	SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo FenceInfo { };
	FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	const bool bSemaphoreImageAvailableCreated = vkCreateSemaphore(m_VulkanLogicalDevice->GetVulkanObject(), &SemaphoreInfo, nullptr, &SemaphoreImageAvailable) != VK_SUCCESS;
	const bool bSemaphoreRenderFinishedCreated = vkCreateSemaphore(m_VulkanLogicalDevice->GetVulkanObject(), &SemaphoreInfo, nullptr, &SemaphoreRenderFinished) != VK_SUCCESS;
	const bool bFenceInFlightCreated = vkCreateFence(m_VulkanLogicalDevice->GetVulkanObject(), &FenceInfo, nullptr, &FenceInFlight) != VK_SUCCESS;

	if (bSemaphoreImageAvailableCreated || bSemaphoreRenderFinishedCreated || bFenceInFlightCreated)
	{
		UNICA_LOG_CRITICAL("Failed to create Vulkan sync objects");
	}
}

VulkanQueueFamilyIndices VulkanInterface::GetDeviceQueueFamilies(const VkPhysicalDevice& VulkanPhysicalDevice)
{
	VulkanQueueFamilyIndices QueueFamilyIndices;

	uint32 QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(VulkanPhysicalDevice, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(VulkanPhysicalDevice, &QueueFamilyCount, QueueFamilies.data());

	uint32 QueueFamilyIndex = 0;
	for (const VkQueueFamilyProperties& QueueFamily : QueueFamilies)
	{
		if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			QueueFamilyIndices.SetGraphicsFamily(QueueFamilyIndex);
		}

		VkBool32 PresentImagesSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(VulkanPhysicalDevice, QueueFamilyIndex, m_VulkanWindowSurface->GetVulkanObject(), &PresentImagesSupport);

		if (PresentImagesSupport)
		{
			QueueFamilyIndices.SetPresentImagesFamily(QueueFamilyIndex);
		}

		if (QueueFamilyIndices.WasSet())
		{
			break;
		}
		
		QueueFamilyIndex++;
	}
	
	return QueueFamilyIndices;
}

VulkanSwapChainSupportDetails VulkanInterface::QuerySwapChainSupport(const VkPhysicalDevice& VulkanPhysicalDevice)
{
	VulkanSwapChainSupportDetails SwapChainSupportDetails;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &SwapChainSupportDetails.SurfaceCapabilities);

	uint32 SurfaceFormatsCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &SurfaceFormatsCount, nullptr);
	if (SurfaceFormatsCount > 0)
	{
		SwapChainSupportDetails.SurfaceFormats.resize(SurfaceFormatsCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &SurfaceFormatsCount, SwapChainSupportDetails.SurfaceFormats.data());
	}

	uint32 PresentModesCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &PresentModesCount, nullptr);
	if (PresentModesCount > 0)
	{
		SwapChainSupportDetails.PresentModes.resize(PresentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &PresentModesCount, SwapChainSupportDetails.PresentModes.data());
	}
	
	return SwapChainSupportDetails;
}

void VulkanInterface::DestroySyncObjects()
{
	vkDestroySemaphore(m_VulkanLogicalDevice->GetVulkanObject(), SemaphoreImageAvailable, nullptr);
	vkDestroySemaphore(m_VulkanLogicalDevice->GetVulkanObject(), SemaphoreRenderFinished, nullptr);
	vkDestroyFence(m_VulkanLogicalDevice->GetVulkanObject(), FenceInFlight, nullptr);
}

void VulkanInterface::Shutdown()
{
	DestroySyncObjects();
	m_VulkanCommandPool->Destroy();
	
	for (const std::unique_ptr<VulkanFramebuffer>& VulkanFramebuffer : m_VulkanFramebuffers)
	{
		VulkanFramebuffer->Destroy();
	}
	
	m_VulkanPipeline->Destroy();
	m_VulkanRenderPass->Destroy();
	
	for (const std::unique_ptr<VulkanImageView>& VulkanImageView : m_VulkanImageViews)
	{
		VulkanImageView->Destroy();
	}
	
	m_VulkanSwapChain->Destroy();
	m_VulkanLogicalDevice->Destroy();
	m_VulkanWindowSurface->Destroy();
	m_VulkanInstance->Destroy();
}
