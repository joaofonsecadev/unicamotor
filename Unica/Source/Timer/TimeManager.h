// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <cstdint>
#include "Subsystem/SubsystemBase.h"

class TimeManager final : public SubsystemBase
{
public:
    float GetDeltaTimeInSeconds();

private:
    void Init() override;
    void Tick() override;
    void Shutdown() override { }
    bool IsTicking() override { return true; }

    uint64_t GetNanoSinceEpoch();
    void CalculateLastFrameTime();

    uint64_t m_LastFrameTimeNano;
    uint64_t m_DeltaTimeNano = 0;
};