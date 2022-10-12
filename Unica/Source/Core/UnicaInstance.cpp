// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "UnicaInstance.h"

#include <csignal>
#include <chrono>

#include "UnicaMinimal.h"
#include "Timer/TimeManager.h"

bool UnicaInstance::m_bHasRequestedExit = false;


void UnicaInstance::Init()
{
    m_SubsystemManager = std::make_unique<SubsystemManager>();
    m_SubsystemManager->Init();
}

void UnicaInstance::TickLogic()
{
	m_SubsystemManager->TickSubsystems();
}

void UnicaInstance::Tick()
{
    if (UnicaSettings::FrameTimeLimit <= 0)
    {
        TickLogic();
        return;
    }

    std::chrono::time_point StartWorkTime = std::chrono::steady_clock::now();
    std::chrono::time_point NextFrameTimeTarget = StartWorkTime + std::chrono::nanoseconds((uint64)(UnicaSettings::FrameTimeLimit * 1'000'000));

    TickLogic();

    std::chrono::time_point FinishWorkTime = std::chrono::steady_clock::now();
    std::chrono::nanoseconds SleepDuration = NextFrameTimeTarget - FinishWorkTime;

    if (SleepDuration <= std::chrono::nanoseconds())
    {
        return;
    }

    while (std::chrono::steady_clock::now() < NextFrameTimeTarget);
}

void UnicaInstance::Shutdown()
{
    m_SubsystemManager->Shutdown();
}
