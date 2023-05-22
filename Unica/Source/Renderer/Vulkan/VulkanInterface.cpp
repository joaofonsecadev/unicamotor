// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanInterface.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "UnicaFileUtilities.h"
#include "UnicaInstance.h"
#include "fmt/format.h"
#include "shaderc/shaderc.hpp"

#include "UnicaMinimal.h"
#include "VulkanQueueFamilyIndices.h"
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
	m_VulkanVertexBuffer->Init();
	m_VulkanCommandBuffer->Init();
	InitSyncObjects();

	UNICA_LOG_INFO("VulkanInterface created successfuly");
}

void VulkanInterface::Tick()
{
	UNICA_PROFILE_FUNCTION
	m_SdlRenderWindow->Tick();
	DrawFrame();

	if (UnicaInstance::HasRequestedExit())
	{
		vkDeviceWaitIdle(m_VulkanLogicalDevice->GetVulkanObject());
	}
}

void VulkanInterface::DrawFrame()
{
	UNICA_PROFILE_FUNCTION
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkWaitForFences");
		vkWaitForFences(m_VulkanLogicalDevice->GetVulkanObject(), 1, &m_FencesInFlight[m_CurrentFrameIndex], VK_TRUE, UINT64_MAX);
	}
	uint32 VulkanImageIndex;
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkAcquireNextImageKHR");
		const VkResult AcquireNextImageResult = vkAcquireNextImageKHR(m_VulkanLogicalDevice->GetVulkanObject(), m_VulkanSwapChain->GetVulkanObject(), UINT64_MAX, m_SemaphoresImageAvailable[m_CurrentFrameIndex], VK_NULL_HANDLE, &VulkanImageIndex);
		if (AcquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChainObjects();
			return;
		}
		else if (AcquireNextImageResult != VK_SUCCESS && AcquireNextImageResult != VK_SUBOPTIMAL_KHR)
		{
			UNICA_LOG_CRITICAL("Couldn't acquire a valid swap chain image");
		}
	}
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkResetFences");
		vkResetFences(m_VulkanLogicalDevice->GetVulkanObject(), 1, &m_FencesInFlight[m_CurrentFrameIndex]);
	}
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkResetCommandBuffer");
		vkResetCommandBuffer(m_VulkanCommandBuffer->GetVulkanCommandBuffersVector()[m_CurrentFrameIndex], 0);
	}
	m_VulkanCommandBuffer->Record(m_CurrentFrameIndex, VulkanImageIndex);

	const VkSemaphore WaitSemaphores[] = { m_SemaphoresImageAvailable[m_CurrentFrameIndex] };
	const VkSemaphore SignalSemaphores[] = { m_SemaphoresRenderFinished[m_CurrentFrameIndex] };
	constexpr VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	
	VkSubmitInfo SubmitInfo { };
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;	
	SubmitInfo.waitSemaphoreCount = 1;
	SubmitInfo.pWaitSemaphores = WaitSemaphores;
	SubmitInfo.pWaitDstStageMask = WaitStages;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &m_VulkanCommandBuffer->GetVulkanCommandBuffersVector()[m_CurrentFrameIndex];
	SubmitInfo.signalSemaphoreCount = 1;
	SubmitInfo.pSignalSemaphores = SignalSemaphores;
	{
		UNICA_PROFILE_FUNCTION_NAMED("vulkan::vkQueueSubmit");
		if (vkQueueSubmit(m_VulkanLogicalDevice->GetVulkanGraphicsQueue(), 1, &SubmitInfo, m_FencesInFlight[m_CurrentFrameIndex]) != VK_SUCCESS)
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
		const VkResult QueuePresentResult = vkQueuePresentKHR(m_VulkanLogicalDevice->GetVulkanPresentImagesQueue(), &VulkanPresentInfo);

		if (QueuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || QueuePresentResult == VK_SUBOPTIMAL_KHR || m_SdlRenderWindow->GetWindowResized())
		{
			m_SdlRenderWindow->SetWindowResized(false);
			RecreateSwapChainObjects();
		}
		else if (QueuePresentResult != VK_SUCCESS)
		{
			UNICA_LOG_CRITICAL("Couldn't present a valid swap chain image");
		}
	}

	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_MaxFramesInFlight;
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
	UNICA_LOG_TRACE("VulkanImageViews created");
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

	UNICA_LOG_TRACE("VulkanFramebuffers created");
}

void VulkanInterface::InitSyncObjects()
{
	m_SemaphoresImageAvailable.resize(m_MaxFramesInFlight);
	m_SemaphoresRenderFinished.resize(m_MaxFramesInFlight);
	m_FencesInFlight.resize(m_MaxFramesInFlight);
	
	VkSemaphoreCreateInfo SemaphoreInfo { };
	SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo FenceInfo { };
	FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint8 FrameIndex = 0; FrameIndex < m_MaxFramesInFlight; FrameIndex++)
	{
		const bool bSemaphoreImageAvailableCreated = vkCreateSemaphore(m_VulkanLogicalDevice->GetVulkanObject(), &SemaphoreInfo, nullptr, &m_SemaphoresImageAvailable[FrameIndex]) != VK_SUCCESS;
		const bool bSemaphoreRenderFinishedCreated = vkCreateSemaphore(m_VulkanLogicalDevice->GetVulkanObject(), &SemaphoreInfo, nullptr, &m_SemaphoresRenderFinished[FrameIndex]) != VK_SUCCESS;
		const bool bFenceInFlightCreated = vkCreateFence(m_VulkanLogicalDevice->GetVulkanObject(), &FenceInfo, nullptr, &m_FencesInFlight[FrameIndex]) != VK_SUCCESS;

		if (bSemaphoreImageAvailableCreated || bSemaphoreRenderFinishedCreated || bFenceInFlightCreated)
		{
			UNICA_LOG_CRITICAL("Failed to create Vulkan sync objects for frame {}", FrameIndex);
		}
	}
}

void VulkanInterface::RecreateSwapChainObjects()
{
    UNICA_PROFILE_FUNCTION

	UNICA_LOG_TRACE("Recreating SwapChainObjects");
	vkDeviceWaitIdle(m_VulkanLogicalDevice->m_VulkanObject);
	DestroySwapChainObjects();
	m_VulkanSwapChain->Init();
	InitVulkanImageViews();
	InitVulkanFramebuffers();
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
	UNICA_LOG_TRACE("Destroying SyncObjects");
	for (uint8 FrameIndex = 0; FrameIndex < m_MaxFramesInFlight; FrameIndex++)
	{
		vkDestroySemaphore(m_VulkanLogicalDevice->GetVulkanObject(), m_SemaphoresImageAvailable[FrameIndex], nullptr);
		vkDestroySemaphore(m_VulkanLogicalDevice->GetVulkanObject(), m_SemaphoresRenderFinished[FrameIndex], nullptr);
		vkDestroyFence(m_VulkanLogicalDevice->GetVulkanObject(), m_FencesInFlight[FrameIndex], nullptr);
	}
}

void VulkanInterface::DestroySwapChainObjects()
{
	UNICA_LOG_TRACE("Destroying SwapChainObjects");
	for (const std::unique_ptr<VulkanFramebuffer>& VulkanFramebuffer : m_VulkanFramebuffers)
	{
		VulkanFramebuffer->Destroy();
	}
	
	for (const std::unique_ptr<VulkanImageView>& VulkanImageView : m_VulkanImageViews)
	{
		VulkanImageView->Destroy();
	}
	
	m_VulkanSwapChain->Destroy();
}

void VulkanInterface::Shutdown()
{
	DestroySwapChainObjects();
	m_VulkanVertexBuffer->Destroy();
	DestroySyncObjects();
	m_VulkanCommandPool->Destroy();	
	m_VulkanPipeline->Destroy();
	m_VulkanRenderPass->Destroy();	
	m_VulkanLogicalDevice->Destroy();
	m_VulkanWindowSurface->Destroy();
	m_VulkanInstance->Destroy();

	UNICA_LOG_INFO("VulkanInterface has been destroyed");
}
