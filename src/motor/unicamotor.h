#pragma once
#include "unicaconfig.h"

class Unicamotor 
{
public:
    Unicamotor() = default;
    explicit Unicamotor(bool request_exit) : m_requested_exit(request_exit) { }
    [[nodiscard]] bool HasExitBeenRequested() const { return m_requested_exit; }

private:
    bool m_requested_exit = false;

    UnicaConfig m_unica_config;
};
