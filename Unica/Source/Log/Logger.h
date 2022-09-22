// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <string>

#define UNICA_LOG(...) Logger::Log(__VA_ARGS__)

enum LogLevel : uint8_t
{
    Log,
    Warning,
    Error,
    Fatal
};

namespace LogCategory
{
    static std::string SubsystemManager = "SubsystemManager";
}

class Logger
{
public:
    static void Log(LogLevel LogLevel, const std::string& LogCategory, const std::string& LogText);
};
