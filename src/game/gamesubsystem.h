#pragma once

#include <memory>
#include <tracy/Tracy.hpp>

#include "subsystem/subsystem.h"

class GameInstance;

class GameSubsystem : public Subsystem
{
public:
    explicit GameSubsystem(Unicamotor* engine);
    ~GameSubsystem() override = default;

    bool ShouldTick() override { return true; }
    std::string GetSubsystemName() override { return "GameSubsystem"; }

    bool Init() override { return true; }
    void Tick() override { ZoneScoped; }

private:
    GameInstance* m_game_instance = nullptr;
};
