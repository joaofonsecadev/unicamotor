#pragma once
#include "unicaconfig.h"
#include "timer/timer.h"

class Unicamotor 
{
public:
    Unicamotor();
    explicit Unicamotor(bool request_exit) : m_requested_exit(request_exit) { }
    [[nodiscard]] bool HasExitBeenRequested() const { return m_requested_exit; }

    void Tick();

private:
    bool m_requested_exit = false;

    UnicaConfig m_unica_config;
    Timer m_timer;
};
