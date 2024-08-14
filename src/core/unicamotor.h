#pragma once

#include <utility>
#include <vector>

enum UnicamotorNetworkMode : uint8_t
{
    Client = 0,
    Standalone,
    Server,
};

class Unicamotor
{
public:
    Unicamotor() = default;
    Unicamotor(const UnicamotorNetworkMode network_mode) : m_network_mode(network_mode) { }

    void Tick();

    static void RequestExit() { m_requested_exit = true; }
    bool HasExitBeenRequested() { return m_requested_exit; }

private:
    UnicamotorNetworkMode m_network_mode = UnicamotorNetworkMode::Standalone;

    static bool m_requested_exit;
};

