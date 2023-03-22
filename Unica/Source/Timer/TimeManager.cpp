// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "TimeManager.h"

#include <ratio>
#include <chrono>

#include "UnicaMinimal.h"

float TimeManager::m_DeltaTimeMillis;
float TimeManager::m_FrameWorkDuration;
float TimeManager::m_FrameSleepDuration;

void TimeManager::Init()
{
	if (std::chrono::steady_clock::period::den != std::nano::den)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "HighResClock unit is not nanoseconds");
		return;
	}
}

void TimeManager::Tick()
{
	UNICA_PROFILE_FUNCTION
    CalculateLastFrameTime();
    //fmt::print(fg(fmt::color::light_gray), "\33[2K\rWorkFrameTime: {:.2f}; SleepFrameTime: {:.2f}; TotalFrameTime: {:.2f}",
    //    m_FrameWorkDuration, m_FrameSleepDuration, m_FrameWorkDuration + m_FrameSleepDuration);
}

void TimeManager::CalculateLastFrameTime()
{
	UNICA_PROFILE_FUNCTION
    const std::chrono::time_point CurrentFrameTime = std::chrono::steady_clock::now();
    const std::chrono::nanoseconds DeltaTime = CurrentFrameTime - m_LastFrameTime;

    m_DeltaTimeMillis = static_cast<float>(DeltaTime.count()) / 1'000'000.f;

    m_LastFrameTime = CurrentFrameTime;
}
