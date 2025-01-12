#include "core/profiling.h"
#include "core/system.h"

#include "motor/unicamotor.h"

int main(int argc, char* argv[])
{
#ifndef WIN32
    printf("This architecture is not supported, RIP");
    return -1;
#endif

    // Setting main thread affinity to the current core
    unc::SystemUtilities::SetCurrentThreadAffinity();

    Unicamotor engine;
    while (!engine.HasExitBeenRequested())
    {
        engine.Tick();
        UnicaProf_FrameMarkNamed("EngineLoop");
    }

    return 0;
}
