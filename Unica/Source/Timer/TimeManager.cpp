// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "TimeManager.h"

#include <ratio>
#include <chrono>

#include "fmt/color.h"

float TimeManager::m_DeltaTimeSeconds;
float TimeManager::m_DeltaTimeMillis;

void TimeManager::Init()
{
    if (std::chrono::high_resolution_clock::period::den != std::nano::den)
    {
        UNICA_LOG(Fatal, "LogTimeManager", "HighResClock unit is not nanoseconds");
    }
    m_LastFrameTimeNanos = GetNanosSinceEpoch();
}

void TimeManager::Tick()
{
    CalculateLastFrameTime();
    //fmt::print(fg(fmt::color::white), "\33[2K\r[LogTimeManager] I'm being ticked at {:.0f} fps", 1 / GetDeltaTimeSeconds());
}

uint64 TimeManager::GetNanosSinceEpoch()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

void TimeManager::CalculateLastFrameTime()
{
    uint64 CurrentFrameTime = GetNanosSinceEpoch();
    m_DeltaTimeNanos = CurrentFrameTime - m_LastFrameTimeNanos;

    // Convert Nanoseconds to Seconds
    m_DeltaTimeSeconds = (float)m_DeltaTimeNanos / 1'000'000'000.f;
    m_DeltaTimeMillis = (float)m_DeltaTimeNanos / 1'000'000.f;

    m_LastFrameTimeNanos = CurrentFrameTime;
}
