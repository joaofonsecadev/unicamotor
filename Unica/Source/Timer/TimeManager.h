// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <cstdint>

#include "UnicaMinimal.h"
#include "Subsystem/SubsystemBase.h"

class TimeManager final : public SubsystemBase
{
public:
    static float GetDeltaTimeSeconds() { return m_DeltaTimeSeconds; }
    static float GetDeltaTimeMillis() { return m_DeltaTimeMillis;  }

private:
    void Init() override;
    void Tick() override;
    void Shutdown() override { }
    bool ShouldTick() override { return true; }

    uint64 GetNanosSinceEpoch();
    void CalculateLastFrameTime();

    uint64 m_LastFrameTimeNanos = 0;
    uint64 m_DeltaTimeNanos = 0;

    static float m_DeltaTimeMillis;
    static float m_DeltaTimeSeconds;
};