#pragma once

#include <string>

class Subsystem
{
public:
    Subsystem() = default;
    virtual ~Subsystem() = 0;

    virtual void Tick() { }

    virtual bool ShouldTick() = 0;
    virtual std::string GetSubsystemName() = 0;
};