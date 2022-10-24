#pragma once

#include <optional>

#include "UnicaMinimal.h"

class VulkanQueueFamilyIndices
{
public:
    void SetGraphicsFamily(uint32 GraphicsFamily) { m_GraphicsFamily = GraphicsFamily; }
    std::optional<uint32> GetGraphicsFamily() { return m_GraphicsFamily; }
    bool WasSet() const { return m_GraphicsFamily.has_value(); }
    
private:
    std::optional<uint32> m_GraphicsFamily;
};
