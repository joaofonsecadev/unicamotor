// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include "Managed/ManagedInterface.h"
#include "Subsystem/SubsystemBase.h"
#include "Vulkan/VulkanInterface.h"

class RenderManager final : public SubsystemBase
{
private:
    void Init() override;
    void Tick() override;
    void Shutdown() override;
    bool ShouldTick() override { return true; }

    std::unique_ptr<RenderInterface> m_RenderInterface = std::make_unique<ManagedInterface>();
};
