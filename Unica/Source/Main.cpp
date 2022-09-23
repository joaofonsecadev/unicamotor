// Copyright joaofonseca.dev, All Rights Reserved.

#include <csignal>
#include "Log/Logger.h"
#include "Subsystem/SubsystemManager.h"

// Temporary way oh handling CRTL+C
void SignalCallbackHandler(int signum)
{
    SubsystemManager::RequestEngineExit();
    // Hacky force of a newline
    UNICA_LOG(Log, "", "");
}

int main(int argc, char* argv[])
{
    signal(SIGINT, SignalCallbackHandler);

    std::unique_ptr<SubsystemManager>SystemManager(new class SubsystemManager);
    SystemManager->Init();

    while (!SubsystemManager::HasRequestedExit())
    {
        SystemManager->TickSubsystems();
    }

    SystemManager->Shutdown();
    return 0;
}
