#include "core/profiling.h"

#include "motor/unicamotor.h"

int main(int argc, char* argv[])
{
    Unicamotor engine(true);
    while (!engine.HasExitBeenRequested())
    {
        UnicaProf_FrameMarkStart("EngineLoop");
        UnicaProf_FrameMarkEnd("EngineLoop");
    }

    return 0;
}
