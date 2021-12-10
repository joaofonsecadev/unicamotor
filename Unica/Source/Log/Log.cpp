// Copyright João Fonseca, All Rights Reserved.

#include "Log.h"

#include <iostream>

void Logger::Log(const LogLevel LogLevel, const string Text)
{
    std::cout << Text << std::endl;
}
