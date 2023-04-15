// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <chrono>

#include "Subsystem/SubsystemBase.h"

class TimeManager final : public SubsystemBase
{
public:
    static float GetDeltaTimeSeconds() { return m_DeltaTimeMillis / 100; }
    static float GetDeltaTimeMillis() { return m_DeltaTimeMillis; }

    static void SetFrameWorkDuration(const float FrameWorkDuration) { m_FrameWorkDuration = FrameWorkDuration; }
    static void SetFrameSleepDuration(const float FrameSleepDuration) { m_FrameSleepDuration = FrameSleepDuration; }

private:
    void Init() override;
    void Tick() override;
    void Shutdown() override { }
    bool ShouldTick() override { return true; }

    void CalculateLastFrameTime();

	std::chrono::steady_clock::time_point m_LastFrameTime = std::chrono::steady_clock::now();

    static float m_DeltaTimeMillis;
    static float m_FrameWorkDuration;
    static float m_FrameSleepDuration;
};
