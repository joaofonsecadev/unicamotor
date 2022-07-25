// Copyright joaofonseca.dev, All Rights Reserved.


#include "Log/Logger.h"
#include "Subsystem/SubsystemManager.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<SubsystemManager>SystemManager(new class SubsystemManager);
    SystemManager->Init();

    UNICA_LOG(Log, "Subsystem initialization finished");

    SystemManager->Shutdown();
    return 0;
}
