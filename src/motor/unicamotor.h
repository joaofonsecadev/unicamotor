#pragma once

class Unicamotor 
{
public:
    bool HasExitBeenRequested() { return m_requested_exit; }

private:
    bool m_requested_exit = false;
};