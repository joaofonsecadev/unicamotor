// Copyright joaofonseca.dev, All Rights Reserved.

#include "UnicaInstance.h"
#include "Subsystem/SubsystemManager.h"

int main(int argc, char* argv[])
{
    std::unique_ptr<UnicaInstance>EngineInstance(new class UnicaInstance);
    EngineInstance->Init();

    while (!UnicaInstance::HasRequestedExit())
    {
        EngineInstance->Tick();
    }

    EngineInstance->Shutdown();
    return 0;
}
