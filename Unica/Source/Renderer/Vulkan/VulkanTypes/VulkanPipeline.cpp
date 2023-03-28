#include "VulkanPipeline.h"

#include "Renderer/Vulkan/VulkanInterface.h"
#include "Renderer/Vulkan/Shaders/ShaderUtilities.h"

void VulkanPipeline::Init()
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
	Viewport.width = static_cast<float>(m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().width);
	Viewport.height = static_cast<float>(m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().height);
	Viewport.minDepth = 0.0f;
	Viewport.maxDepth = 1.0f;

	VkRect2D ScissorRectangle{};
	ScissorRectangle.offset = {0, 0};
	ScissorRectangle.extent = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent();

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

	if (vkCreatePipelineLayout(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &PipelineLayoutInfo, nullptr, &m_VulkanPipelineLayout) != VK_SUCCESS)
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
	GraphicsPipelineCreateInfo.renderPass = m_OwningVulkanAPI->GetVulkanRenderPass()->GetVulkanObject();
	GraphicsPipelineCreateInfo.subpass = 0;

	if (vkCreateGraphicsPipelines(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), VK_NULL_HANDLE, 1, &GraphicsPipelineCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create the VulkanGraphicsPipeline");
	}

	// Cleanup shader modules since they've already been created
	vkDestroyShaderModule(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), VertShaderModule, nullptr);
	vkDestroyShaderModule(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), FragShaderModule, nullptr);
}

VkShaderModule VulkanPipeline::CreateShaderModule(const std::vector<char>& ShaderBinary)
{
	VkShaderModuleCreateInfo ShaderModuleCreateInfo { };
	ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderModuleCreateInfo.codeSize = ShaderBinary.size();
	ShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32*>(ShaderBinary.data());

	VkShaderModule ShaderModule;
	if (vkCreateShaderModule(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &ShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create a VulkanShaderModule");
	}
	
	return ShaderModule;
}

void VulkanPipeline::Destroy()
{
	vkDestroyPipeline(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
	vkDestroyPipelineLayout(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanPipelineLayout, nullptr);
}
