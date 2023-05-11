#pragma once

#include <array>

#include "vulkan/vulkan_core.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct VulkanVertex
{
    glm::vec2 Position;
    glm::vec3 Color;

    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
};
