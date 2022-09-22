// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <string>

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
    static void Log(LogLevel LogLevel, const std::string& Text);
};

#define UNICA_LOG(...) Logger::Log(__VA_ARGS__)