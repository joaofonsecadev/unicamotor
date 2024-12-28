#include "core/profiling.h"

#include "core/string.h"
#include "motor/unicamotor.h"

int main(int argc, char* argv[])
{
    Unicamotor engine(true);
    while (!engine.HasExitBeenRequested())
    {
        UnicaProf_FrameMarkStart("EngineLoop");
        unc::string teste_string = unc::string("o meu nome e rebeca e o teu qual e");
        unc::string tesss = unc::string("quem me dera algo doce");
        UnicaProf_FrameMarkEnd("EngineLoop");
    }

    return 0;
}
