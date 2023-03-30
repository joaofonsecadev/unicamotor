// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "UnicaInstance.h"

#include <chrono>
#include <fstream>

#include "UnicaMinimal.h"
#include "UnicaFileUtilities.h"
#include "Timer/TimeManager.h"

bool UnicaInstance::m_bHasRequestedExit = false;
std::filesystem::path UnicaInstance::m_ProjectRootDirectory;

void UnicaInstance::Init()
{
    Logger::Init();
    
    m_SubsystemManager = std::make_unique<SubsystemManager>();
    m_SubsystemManager->Init();
}

void UnicaInstance::TickLogic()
{
    UNICA_PROFILE_FUNCTION
	m_SubsystemManager->TickSubsystems();
}

void UnicaInstance::Tick()
{
    UNICA_PROFILE_FUNCTION
    
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

void UnicaInstance::SetProjectRootDirectory(char* SystemStyledExecutableDirectory)
{
    const std::filesystem::path ExecutablePath(SystemStyledExecutableDirectory);

    // TODO: This is only valid for development. Must adapt for the packaging system
    m_ProjectRootDirectory = ExecutablePath.parent_path().parent_path().parent_path();
}

void UnicaInstance::Shutdown()
{
    m_SubsystemManager->Shutdown();
}
