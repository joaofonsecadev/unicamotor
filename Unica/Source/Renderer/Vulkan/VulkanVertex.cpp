#include "VulkanVertex.h"

VkVertexInputBindingDescription VulkanVertex::GetBindingDescription()
{
    VkVertexInputBindingDescription BindingDescription { };
    BindingDescription.binding = 0;
    BindingDescription.stride = sizeof(VulkanVertex);
    BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
    return BindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> VulkanVertex::GetAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 2> AttributeDescriptions { };

    // Position Descriptor
    AttributeDescriptions[0].binding = 0;
    AttributeDescriptions[0].location = 0;
    AttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    AttributeDescriptions[0].offset = offsetof(VulkanVertex, Position);

    // Color Descriptor
    AttributeDescriptions[1].binding = 0;
    AttributeDescriptions[1].location = 1;
    AttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    AttributeDescriptions[1].offset = offsetof(VulkanVertex, Color);
}
