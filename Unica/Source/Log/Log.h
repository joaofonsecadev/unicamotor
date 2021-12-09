// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "UnicaPCH.h"

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
    static void Log(LogLevel LogLevel, const string& Text);
};