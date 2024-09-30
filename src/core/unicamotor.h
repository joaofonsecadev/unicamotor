#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

#include "subsystem/subsystem.h"

class TimerSubsystem;

enum UnicamotorNetworkMode : uint8_t
{
    Client = 0,
    Standalone,
    Server,
};

class Unicamotor
{
public:
    Unicamotor();
    explicit Unicamotor(const UnicamotorNetworkMode network_mode) : Unicamotor() { m_network_mode = network_mode; }

    void Tick();

    static void RequestExit() { m_requested_exit = true; }
    static bool HasExitBeenRequested() { return m_requested_exit; }

private:
    void InitializeSubsystems();
    void ResolveGraphicsApi();

    UnicamotorNetworkMode m_network_mode = UnicamotorNetworkMode::Standalone;
    std::unordered_map<std::string, Subsystem*> m_subsystems_map;
    std::vector<std::unique_ptr<Subsystem>> m_subsystems_vector;

    TimerSubsystem* m_timer_subsystem = nullptr;

    static bool m_requested_exit;
};

