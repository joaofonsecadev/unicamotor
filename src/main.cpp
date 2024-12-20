#include "core/profiling.h"

#include "motor/unicamotor.h"

int main(int argc, char* argv[])
{
    Unicamotor engine;
    while (!engine.HasExitBeenRequested())
    {
        UnicaProf_FrameMarkNamed("EngineLoop");
    }

    return 0;
}
