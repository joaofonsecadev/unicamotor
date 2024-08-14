#pragma once

#include <string>

class Unicamotor;

class Subsystem
{
public:
    Subsystem() = delete;
    explicit Subsystem(Unicamotor* engine_owner) : m_engine(engine_owner) { }
    virtual ~Subsystem() = default;

    virtual bool ShouldTick() = 0;
    virtual std::string GetSubsystemName() = 0;

    virtual void Tick() { }

private:
    Unicamotor* m_engine = nullptr;
};