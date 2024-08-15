#pragma once

#include "subsystem/subsystem.h"

class RendererSubsystem : public Subsystem
{
public:
    RendererSubsystem() = delete;
    explicit RendererSubsystem(Unicamotor* engine) : Subsystem(engine) { }
    ~RendererSubsystem() override = default;

    bool ShouldTick() override { return true; }
    std::string GetSubsystemName() override { return "RendererSubsystem"; }

    void Tick() override = 0;
};
