#pragma once

#include <utility>
#include <vector>

enum UnicamotorNetworkMode : uint8_t
{
    Client = 0,
    Standalone,
    Server,
};

struct UnicamotorConfig
{

    std::vector<std::string> command_line_arguments;
};

class Unicamotor
{
public:
    Unicamotor() = default;
    Unicamotor(UnicamotorConfig config) : m_unicamotor_config(std::move(config)) { }

private:
    UnicamotorConfig m_unicamotor_config;
};

