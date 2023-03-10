// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "UnicaInstance.h"

#include <chrono>
#include <fstream>

#include "UnicaMinimal.h"
#include "UnicaSystemUtilities.h"
#include "Timer/TimeManager.h"

bool UnicaInstance::m_bHasRequestedExit = false;
std::filesystem::path UnicaInstance::m_ExecutableDirectory;

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

    const std::chrono::time_point StartWorkTime = std::chrono::steady_clock::now();
    const std::chrono::time_point NextFrameTimeTarget = StartWorkTime + std::chrono::nanoseconds(static_cast<uint64>(UnicaSettings::FrameTimeLimit * 1'000'000));

    TickLogic();

    const std::chrono::time_point FinishWorkTime = std::chrono::steady_clock::now();
    const std::chrono::nanoseconds SleepDuration = NextFrameTimeTarget - FinishWorkTime;
    TimeManager::SetFrameWorkDuration(static_cast<float>((FinishWorkTime - StartWorkTime).count()) / 1'000'000);

    if (SleepDuration <= std::chrono::nanoseconds())
    {
		TimeManager::SetFrameSleepDuration(0);
        return;
    }
    
	TimeManager::SetFrameSleepDuration(static_cast<float>(SleepDuration.count()) / 1'000'000);
    while (std::chrono::steady_clock::now() < NextFrameTimeTarget);
}

void UnicaInstance::SetUnicaRootDirectory(char* SystemStyledExecutableDirectory)
{
    const std::filesystem::path ExecutablePath(SystemStyledExecutableDirectory);

    // TODO: This is only valid for development. Must adapt for the packaging system
    m_ExecutableDirectory = ExecutablePath.parent_path().parent_path().parent_path();
}

void UnicaInstance::Shutdown()
{
    m_SubsystemManager->Shutdown();
}
