// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "UnicaInstance.h"

#include <csignal>

#include "UnicaMinimal.h"

bool UnicaInstance::m_bHasRequestedExit = false;

void UnicaInstance::Init()
{
    m_SubsystemManager = std::make_unique<SubsystemManager>();
    m_SubsystemManager->Init();
}

void UnicaInstance::Tick()
{
    m_SubsystemManager->TickSubsystems();
}

void UnicaInstance::Shutdown()
{
    m_SubsystemManager->Shutdown();
}
