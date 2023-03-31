// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "TimeManager.h"

#include "UnicaMinimal.h"

float TimeManager::m_DeltaTimeMillis;
float TimeManager::m_FrameWorkDuration;
float TimeManager::m_FrameSleepDuration;

void TimeManager::Init()
{
	if constexpr (std::chrono::steady_clock::period::den != std::nano::den)
	{
		UNICA_LOG(spdlog::level::critical, "HighResClock unit is not nanoseconds");
		return;
	}
}

void TimeManager::Tick()
{
	UNICA_PROFILE_FUNCTION
    CalculateLastFrameTime();
}

void TimeManager::CalculateLastFrameTime()
{
	UNICA_PROFILE_FUNCTION
    const std::chrono::time_point CurrentFrameTime = std::chrono::steady_clock::now();
    const std::chrono::nanoseconds DeltaTime = CurrentFrameTime - m_LastFrameTime;

    m_DeltaTimeMillis = static_cast<float>(DeltaTime.count()) / 1'000'000.f;

    m_LastFrameTime = CurrentFrameTime;
}
