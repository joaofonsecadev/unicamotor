// Copyright João Fonseca, All Rights Reserved.

#include "Engine/UnicaEngine.h"


int main(int argc, char* argv[])
{
    UnicaEngine EngineInstance;
    
    EngineInstance.Init();
    
    while (!EngineInstance.GetRequestedExit())
    {
        EngineInstance.Tick();
    }

    EngineInstance.Exit();
    return 0;
}
