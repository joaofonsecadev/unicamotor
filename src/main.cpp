#include "tracy/Tracy.hpp"

#include "motor/unicamotor.h"

int main(int argc, char* argv[])
{
    Unicamotor engine;
    while (!engine.HasExitBeenRequested())
    {
        FrameMarkNamed("EngineLoop");
    }

    return 0;
}
