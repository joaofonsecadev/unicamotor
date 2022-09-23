// Copyright joaofonseca.dev, All Rights Reserved.

#include "UnicaInstance.h"

#include <csignal>

#include "Log/Logger.h"

bool UnicaInstance::m_bHasRequestedExit = false;

void UnicaInstance::Init()
{
    signal(SIGINT, SignalCallbackHandler);
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

void UnicaInstance::SignalCallbackHandler(int signum)
{
    UnicaInstance::RequestExit();
    // Hacky force of a newline
    UNICA_LOG(Log, "", "");
}
