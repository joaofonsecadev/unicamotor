// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include <iostream>

#include "UnicaInstance.h"
#include "Subsystem/SubsystemManager.h"

int main(int argc, char* argv[])
{
    const std::unique_ptr<UnicaInstance> EngineInstance(new class UnicaInstance);
    
    EngineInstance->SetUnicaRootDirectory(/* SystemStyledExecutableDirectory */ argv[0]);
    EngineInstance->Init();

    while (!UnicaInstance::HasRequestedExit())
    {
        EngineInstance->Tick();
    }

    EngineInstance->Shutdown();
    return 0;
}
