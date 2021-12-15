// Copyright João Fonseca, All Rights Reserved.

#include "UnicaPch.h"
#include "Log.h"

#include <iostream>

void Logger::Log(const LogLevel LogLevel, const string Text)
{
    std::cout << Text << std::endl;
}
