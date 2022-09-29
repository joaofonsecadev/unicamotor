// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "TimeManager.h"

#include <ratio>
#include <chrono>

#include "fmt/color.h"

float TimeManager::m_DeltaTimeSeconds;

void TimeManager::Init()
{
    if (std::chrono::high_resolution_clock::period::den != std::nano::den)
    {
        UNICA_LOG(Fatal, "LogTimeManager", "HighResClock unit is not nanoseconds. Aborting execution");
    }
    m_LastFrameTimeNano = GetNanoSinceEpoch();
}

void TimeManager::Tick()
{
    CalculateLastFrameTime();
    //fmt::print(fg(fmt::color::white), "\33[2K\r[LogTimeManager] I'm being ticked at {:.0f} fps", 1 / GetDeltaTimeSeconds());
}

uint64 TimeManager::GetNanoSinceEpoch()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

void TimeManager::CalculateLastFrameTime()
{
    uint64 CurrentFrameTime = GetNanoSinceEpoch();
    m_DeltaTimeNano = CurrentFrameTime - m_LastFrameTimeNano;

    // Convert Nanoseconds to Seconds
    m_DeltaTimeSeconds = (float)m_DeltaTimeNano / 1'000'000'000.f;
    m_LastFrameTimeNano = CurrentFrameTime;
}
