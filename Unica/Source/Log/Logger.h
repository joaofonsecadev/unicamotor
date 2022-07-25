// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <string>
#include "UnicaMinimal.h"

enum LogLevel : uint8
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