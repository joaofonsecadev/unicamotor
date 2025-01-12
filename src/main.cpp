#include "../external/glfw/src/internal.h"
#include "core/profiling.h"
#include "core/system.h"

#include "motor/unicamotor.h"

int main(int argc, char* argv[])
{
    unc::SystemUtilities::SetCurrentThreadAffinity();
#ifndef WIN32
    printf("This architecture is not supported, RIP");
    return -1;
#endif

    Unicamotor engine(true);
    while (!engine.HasExitBeenRequested())
    {
        UnicaProf_FrameMarkStart("EngineLoop");
        UnicaProf_FrameMarkEnd("EngineLoop");
    }

    return 0;
}
