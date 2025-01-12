#include "unicamotor.h"
#include "core/profiling.h"

Unicamotor::Unicamotor()
{
    //@TODO get config from toml file
    m_timer.SetMinimumFrameTime(m_unica_config.core_max_frame_time_ms);
}

void Unicamotor::Tick()
{
    UnicaProf_ZoneScoped;

    m_timer.MarkStartWork();

    m_timer.MarkEndWork();
    m_timer.PossibleSleep();
}
