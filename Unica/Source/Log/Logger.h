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

class Logger
{
public:
    static void Log(LogLevel LogLevel, const std::string& LogCategory, const std::string& LogText);
};
