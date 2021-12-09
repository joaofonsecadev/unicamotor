// Copyright João Fonseca, All Rights Reserved.

#include "Engine/UnicaEngine.h"

UnicaEngine GEngine;
int main(int argc, char* argv[])
{
    GEngine.Init();
    
    while (!GEngine.GetRequestedExit())
    {
        GEngine.Tick();
    }

    GEngine.Exit();
    return 0;
}
