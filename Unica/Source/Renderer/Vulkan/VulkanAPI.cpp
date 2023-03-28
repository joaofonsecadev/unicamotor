// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanAPI.h"

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

void VulkanAPI::Init()
{
	m_VulkanInstance->Init();
	m_VulkanWindowSurface->Init();
	m_VulkanPhysicalDevice->Init();
	m_VulkanLogicalDevice->Init();
	m_VulkanSwapChain->Init();
	InitVulkanImageViews();
	m_VulkanRenderPass->Init();
	CreateGraphicsPipeline();
	CreateFramebuffers();
	CreateCommandPool();
}

void VulkanAPI::Tick()
{
	m_SdlRenderWindow->Tick();
}

VulkanQueueFamilyIndices VulkanAPI::GetDeviceQueueFamilies(const VkPhysicalDevice& VulkanPhysicalDevice)
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

VulkanSwapChainSupportDetails VulkanAPI::QuerySwapChainSupport(const VkPhysicalDevice& VulkanPhysicalDevice)
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

void VulkanAPI::InitVulkanImageViews()
{
	uint32 SwapChainImageIteration = 0;
	m_VulkanSwapChainImageViews.resize(m_VulkanSwapChain->GetVulkanSwapChainImages().size());
	for (const VkImage& SwapChainImage : m_VulkanSwapChain->GetVulkanSwapChainImages())
	{
		m_VulkanSwapChainImageViews[SwapChainImageIteration] = std::make_unique<VulkanImageView>(this, SwapChainImage);
		m_VulkanSwapChainImageViews[SwapChainImageIteration]->Init();
		SwapChainImageIteration++;
	}
}

void VulkanAPI::CreateGraphicsPipeline()
{
	std::vector<char> VertShaderBinary = ShaderUtilities::LoadShader("Engine:Shaders/shader.vert");
	std::vector<char> FragShaderBinary = ShaderUtilities::LoadShader("Engine:Shaders/shader.frag");

	VkShaderModule VertShaderModule = CreateShaderModule(VertShaderBinary);
	VkShaderModule FragShaderModule = CreateShaderModule(FragShaderBinary);

	VkPipelineShaderStageCreateInfo VertPipelineShaderStageCreateInfo { };
	VertPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	VertPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	VertPipelineShaderStageCreateInfo.module = VertShaderModule;
	VertPipelineShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FragPipelineShaderStageCreateInfo { };
	FragPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FragPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	FragPipelineShaderStageCreateInfo.module = FragShaderModule;
	FragPipelineShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfos[] = { VertPipelineShaderStageCreateInfo, FragPipelineShaderStageCreateInfo };

	std::vector<VkDynamicState> PipelineDynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo PipelineDynamicCreateInfo { };
	PipelineDynamicCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	PipelineDynamicCreateInfo.dynamicStateCount = static_cast<uint32_t>(PipelineDynamicStates.size());
	PipelineDynamicCreateInfo.pDynamicStates = PipelineDynamicStates.data();

	VkPipelineVertexInputStateCreateInfo PipelineVertexInputCreateInfo { };
	PipelineVertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	PipelineVertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	PipelineVertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
	PipelineVertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	PipelineVertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyCreateInfo{};
	PipelineInputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	PipelineInputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	PipelineInputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport Viewport{};
	Viewport.x = 0.0f;
	Viewport.y = 0.0f;
	Viewport.width = static_cast<float>(m_VulkanSwapChain->GetVulkanExtent().width);
	Viewport.height = static_cast<float>(m_VulkanSwapChain->GetVulkanExtent().height);
	Viewport.minDepth = 0.0f;
	Viewport.maxDepth = 1.0f;

	VkRect2D ScissorRectangle{};
	ScissorRectangle.offset = {0, 0};
	ScissorRectangle.extent = m_VulkanSwapChain->GetVulkanExtent();

	VkPipelineViewportStateCreateInfo PipelineViewportCreateInfo { };
	PipelineViewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	PipelineViewportCreateInfo.viewportCount = 1;
	PipelineViewportCreateInfo.pViewports = &Viewport;
	PipelineViewportCreateInfo.scissorCount = 1;
	PipelineViewportCreateInfo.pScissors = &ScissorRectangle;

	VkPipelineRasterizationStateCreateInfo PipelineRasterizationCreateInfo { };
	PipelineRasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	PipelineRasterizationCreateInfo.depthClampEnable = VK_FALSE;
	PipelineRasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	PipelineRasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	PipelineRasterizationCreateInfo.lineWidth = 1.0f;
	PipelineRasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	PipelineRasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	PipelineRasterizationCreateInfo.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo PipelineMultisampleCreateInfo { };
	PipelineMultisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	PipelineMultisampleCreateInfo.sampleShadingEnable = VK_FALSE;
	PipelineMultisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	PipelineMultisampleCreateInfo.minSampleShading = 1.0f;
	PipelineMultisampleCreateInfo.pSampleMask = nullptr;
	PipelineMultisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
	PipelineMultisampleCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState PipelineColorBlendAttachment { };
	PipelineColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	PipelineColorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo PipelineColorBlend { };
	PipelineColorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	PipelineColorBlend.logicOpEnable = VK_FALSE;
	PipelineColorBlend.attachmentCount = 1;
	PipelineColorBlend.pAttachments = &PipelineColorBlendAttachment;

	VkPipelineLayoutCreateInfo PipelineLayoutInfo { };
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	if (vkCreatePipelineLayout(m_VulkanLogicalDevice->GetVulkanObject(), &PipelineLayoutInfo, nullptr, &m_VulkanPipelineLayout) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create a VulkanPipelineLayout");
	}

	VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo { };
	GraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	GraphicsPipelineCreateInfo.stageCount = 2;
	GraphicsPipelineCreateInfo.pStages = PipelineShaderStageCreateInfos;
	GraphicsPipelineCreateInfo.pVertexInputState = &PipelineVertexInputCreateInfo;
	GraphicsPipelineCreateInfo.pInputAssemblyState = &PipelineInputAssemblyCreateInfo;
	GraphicsPipelineCreateInfo.pViewportState = &PipelineViewportCreateInfo;
	GraphicsPipelineCreateInfo.pRasterizationState = &PipelineRasterizationCreateInfo;
	GraphicsPipelineCreateInfo.pMultisampleState = &PipelineMultisampleCreateInfo;
	GraphicsPipelineCreateInfo.pColorBlendState = &PipelineColorBlend;
	GraphicsPipelineCreateInfo.pDynamicState = &PipelineDynamicCreateInfo;
	GraphicsPipelineCreateInfo.layout = m_VulkanPipelineLayout;
	GraphicsPipelineCreateInfo.renderPass = m_VulkanRenderPass->GetVulkanObject();
	GraphicsPipelineCreateInfo.subpass = 0;

	if (vkCreateGraphicsPipelines(m_VulkanLogicalDevice->GetVulkanObject(), VK_NULL_HANDLE, 1, &GraphicsPipelineCreateInfo, nullptr, &m_VulkanGraphicsPipeline) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create the VulkanGraphicsPipeline");
	}

	// Cleanup shader modules since they've already been created
	vkDestroyShaderModule(m_VulkanLogicalDevice->GetVulkanObject(), VertShaderModule, nullptr);
	vkDestroyShaderModule(m_VulkanLogicalDevice->GetVulkanObject(), FragShaderModule, nullptr);
}

VkShaderModule VulkanAPI::CreateShaderModule(const std::vector<char>& ShaderBinary)
{
	VkShaderModuleCreateInfo ShaderModuleCreateInfo { };
	ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderModuleCreateInfo.codeSize = ShaderBinary.size();
	ShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32*>(ShaderBinary.data());

	VkShaderModule ShaderModule;
	if (vkCreateShaderModule(m_VulkanLogicalDevice->GetVulkanObject(), &ShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create a VulkanShaderModule");
	}
	
	return ShaderModule;
}

void VulkanAPI::CreateFramebuffers()
{
	uint32 SwapChainImageViewLoopIndex = 0;
	m_SwapChainFramebuffers.resize(m_VulkanSwapChainImageViews.size());
	for (const std::unique_ptr<VulkanImageView>& SwapChainImageView : m_VulkanSwapChainImageViews)
	{
		const VkImageView ImageViewAttachments[] = { SwapChainImageView->GetVulkanObject() };
		VkFramebufferCreateInfo FramebufferCreateInfo { };
		FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferCreateInfo.renderPass = m_VulkanRenderPass->GetVulkanObject();
		FramebufferCreateInfo.attachmentCount = 1;
		FramebufferCreateInfo.pAttachments = ImageViewAttachments;
		FramebufferCreateInfo.width = m_VulkanSwapChain->GetVulkanExtent().width;
		FramebufferCreateInfo.height = m_VulkanSwapChain->GetVulkanExtent().height;
		FramebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(m_VulkanLogicalDevice->GetVulkanObject(), &FramebufferCreateInfo, nullptr, &m_SwapChainFramebuffers[SwapChainImageViewLoopIndex]) != VK_SUCCESS)
		{
			UNICA_LOG(spdlog::level::critical, "Failed to create a VulkanFramebuffer");
		}
		
		SwapChainImageViewLoopIndex++;
	}
}

void VulkanAPI::CreateCommandPool()
{
	VulkanQueueFamilyIndices QueueFamilyIndices = GetDeviceQueueFamilies(m_VulkanPhysicalDevice->GetVulkanObject());

	VkCommandPoolCreateInfo CommandPoolCreateInfo { };
	CommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	CommandPoolCreateInfo.queueFamilyIndex = QueueFamilyIndices.GetGraphicsFamily().value();

	if (vkCreateCommandPool(m_VulkanLogicalDevice->GetVulkanObject(), &CommandPoolCreateInfo, nullptr, &m_VulkanCommandPool) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create the VulkanCommandPool");
	}
}

void VulkanAPI::Shutdown()
{
	vkDestroyCommandPool(m_VulkanLogicalDevice->GetVulkanObject(), m_VulkanCommandPool, nullptr);
	for (VkFramebuffer_T* SwapChainFramebuffer : m_SwapChainFramebuffers)
	{
		vkDestroyFramebuffer(m_VulkanLogicalDevice->GetVulkanObject(), SwapChainFramebuffer, nullptr);
	}
	vkDestroyPipeline(m_VulkanLogicalDevice->GetVulkanObject(), m_VulkanGraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(m_VulkanLogicalDevice->GetVulkanObject(), m_VulkanPipelineLayout, nullptr);

	m_VulkanRenderPass->Destroy();
	
	for (const std::unique_ptr<VulkanImageView>& VulkanImageView : m_VulkanSwapChainImageViews)
	{
		VulkanImageView->Destroy();
	}
	
	m_VulkanSwapChain->Destroy();
	m_VulkanLogicalDevice->Destroy();
	m_VulkanWindowSurface->Destroy();
	m_VulkanInstance->Destroy();
}
