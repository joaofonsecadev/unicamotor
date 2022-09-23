// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <cstdint>
#include "UnicaMinimal.h"
#include "Subsystem/SubsystemBase.h"

class TimeManager final : public SubsystemBase
{
public:
    static float GetDeltaTimeSeconds() { return m_DeltaTimeSeconds; }

private:
    void Init() override;
    void Tick() override;
    void Shutdown() override { }
    bool ShouldTick() override { return true; }

    uint64 GetNanoSinceEpoch();
    void CalculateLastFrameTime();

    uint64 m_LastFrameTimeNano;
    uint64 m_DeltaTimeNano = 0;

    static float m_DeltaTimeSeconds;
};