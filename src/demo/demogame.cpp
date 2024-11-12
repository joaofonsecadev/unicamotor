#include "demogame.h"

GameInstance* DemoGame::GetDemoGameInstance(GameSubsystem* owner)
{
    static GameInstance* game_instance = new DemoGame(owner);
    return game_instance;
}
