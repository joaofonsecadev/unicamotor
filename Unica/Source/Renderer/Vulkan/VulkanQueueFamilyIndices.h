// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <optional>

#include "UnicaMinimal.h"

class VulkanQueueFamilyIndices
{
public:
    void SetGraphicsFamily(uint32 GraphicsFamily) { m_GraphicsFamily = GraphicsFamily; }
    std::optional<uint32> GetGraphicsFamily() { return m_GraphicsFamily; }

    void SetPresentImagesFamily(uint32 PresentImagesFamily) { m_PresentImagesFamily = PresentImagesFamily; }
    std::optional<uint32> GetPresentImagesFamily() const { return m_PresentImagesFamily; }
    
    bool WasSet() const { return m_GraphicsFamily.has_value() && m_PresentImagesFamily.has_value(); }
    
private:
    std::optional<uint32> m_GraphicsFamily;
    std::optional<uint32> m_PresentImagesFamily;
};
