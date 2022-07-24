// Copyright joaofonseca.dev, All Rights Reserved.

#include "Logger.h"

#include <iostream>

void Logger::Log(LogLevel LogLevel, const std::string& Text)
{
    std::cout << Text << std::endl;
}

