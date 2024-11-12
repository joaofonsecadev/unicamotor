#include "gamesubsystem.h"

class GameInstance
{
public:
    GameInstance() = default;
    explicit GameInstance(GameSubsystem* owner) : m_owner(owner) { }

private:
    GameSubsystem* m_owner = nullptr;
};
