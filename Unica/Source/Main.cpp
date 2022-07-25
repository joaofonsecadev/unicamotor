// Copyright joaofonseca.dev, All Rights Reserved.

#include <memory>
#include "Subsystem/SubsystemManager.h"
#include "Log/Logger.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<SubsystemManager> SubsystemManager(new class SubsystemManager);
    SubsystemManager->Init();

    UNICA_LOG(Log, "Subsystem initialization finished");

    SubsystemManager->Shutdown();
    return 0;
}
