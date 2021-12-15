// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "Core/UnicaPch.h"

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
    static void Log(LogLevel LogLevel, string Text);
};

#define UNICA_LOG(...) Logger::Log(__VA_ARGS__)
