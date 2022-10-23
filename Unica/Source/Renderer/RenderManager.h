// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <memory>

#include "Subsystem/SubsystemBase.h"
#include "RenderWindow.h"
#include "Vulkan/VulkanAPI.h"

class RenderManager final : public SubsystemBase
{
private:
    void Init() override;
    void Tick() override;
    void Shutdown() override { }
    bool ShouldTick() override { return true; }

    std::unique_ptr<RenderWindow> m_RenderWindow;
    std::unique_ptr<VulkanAPI> m_RenderInterface;
};
