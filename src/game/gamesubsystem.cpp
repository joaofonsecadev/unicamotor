#include "gamesubsystem.h"

#ifndef HAS_PROJECT_LIB
#include "demo/demogame.h"
#else
namespace UnicaGame
{
extern GameInstance* GetGameInstance(GameSubsystem* owner);
}
#endif

GameSubsystem::GameSubsystem(Unicamotor *engine) : Subsystem(engine)
{
#ifdef HAS_PROJECT_LIB
    m_game_instance = UnicaGame::GetGameInstance(this);
#else
    m_game_instance = DemoGame::GetDemoGameInstance(this);
#endif
}
