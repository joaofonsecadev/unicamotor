// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <memory>

#include "RenderInterface.h"
#include "Subsystem/SubsystemBase.h"

class RenderManager final : public SubsystemBase
{
private:
    void Init() override;
    void Tick() override;
    void Shutdown() override;
    bool ShouldTick() override { return true; }

    std::unique_ptr<RenderInterface> m_RenderInterface;
};
