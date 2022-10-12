// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <cstdint>
#include <chrono>

#include "UnicaMinimal.h"
#include "Subsystem/SubsystemBase.h"

class TimeManager final : public SubsystemBase
{
public:
    static float GetDeltaTimeSeconds() { return m_DeltaTimeMillis / 1000; }
    static float GetDeltaTimeMillis() { return m_DeltaTimeMillis;  }

private:
    void Init() override;
    void Tick() override;
    void Shutdown() override { }
    bool ShouldTick() override { return true; }

    void CalculateLastFrameTime();

	std::chrono::steady_clock::time_point m_LastFrameTime = std::chrono::steady_clock::now();

    static float m_DeltaTimeMillis;
};