// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "UnicaInstance.h"

#include "UnicaMinimal.h"
#include "UnicaSettings.h"
#include "Timer/TimeManager.h"

bool UnicaInstance::m_bHasRequestedExit = false;
std::filesystem::path UnicaInstance::m_ProjectRootDirectory;

void UnicaInstance::Init()
{
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
    Sleep(NextFrameTimeTarget.time_since_epoch().count());
}

void UnicaInstance::Sleep(const uint64 NextFrameTimeTargetCount)
{
    UNICA_PROFILE_FUNCTION
    while (std::chrono::steady_clock::now().time_since_epoch().count() < NextFrameTimeTargetCount);
}

void UnicaInstance::SetProjectRootDirectory(char* SystemStyledExecutableDirectory)
{
    const std::filesystem::path ExecutablePath(SystemStyledExecutableDirectory);
    std::filesystem::path PathBeingValidated = ExecutablePath;

    while (true)
    {
        if (ExecutablePath == ExecutablePath.root_path())
        {
            UNICA_LOG_CRITICAL("Can't find a valid project root directory. Is the project binary in the correct directory?");
        }

        std::filesystem::path BaseEnginePath = PathBeingValidated;
        BaseEnginePath.append("Unica/Config/BaseEngine.ini").make_preferred();
        if (!exists(BaseEnginePath))
        {
            UNICA_LOG_TRACE("Path '{}' is not a valid project root path", PathBeingValidated.string());
            PathBeingValidated = PathBeingValidated.parent_path();
            BaseEnginePath = PathBeingValidated;
            continue;
        }

        UNICA_LOG_DEBUG("Found a valid Unica path at '{}'", PathBeingValidated.string());
        break;
    }

    m_ProjectRootDirectory = PathBeingValidated;
}

void UnicaInstance::Shutdown()
{
    m_SubsystemManager->Shutdown();
}
