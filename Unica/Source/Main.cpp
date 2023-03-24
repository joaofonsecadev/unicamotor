// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include <iostream>

#include "UnicaMinimal.h"
#include "UnicaInstance.h"

int main(int argc, char* argv[])
{
    const std::unique_ptr<UnicaInstance> EngineInstance(new class UnicaInstance);
    
    EngineInstance->SetProjectRootDirectory(/* SystemStyledExecutableDirectory */ argv[0]);
    EngineInstance->Init();

    while (!UnicaInstance::HasRequestedExit())
    {
        EngineInstance->Tick();
        UNICA_PROFILE_FRAME("EngineLoop");
    }

    EngineInstance->Shutdown();
    return 0;
}
