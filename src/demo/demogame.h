#pragma once

#include "game/gameinstance.h"

class GameSubsystem;

class DemoGame : public GameInstance
{
public:
    explicit DemoGame(GameSubsystem* owner) : GameInstance(owner) { }

    static GameInstance* GetDemoGameInstance(GameSubsystem* owner);
};
