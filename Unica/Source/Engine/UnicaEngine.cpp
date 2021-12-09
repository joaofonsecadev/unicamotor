// Copyright João Fonseca, All Rights Reserved.

#include "UnicaEngine.h"

#include "Log/Log.h"

void UnicaEngine::Init()
{
    Logger::Log(Log, "Initiating engine");
}

void UnicaEngine::Tick()
{
    Logger::Log(Log, "Ticking engine");
    m_bRequestedExit = true;
}

void UnicaEngine::Exit()
{
    Logger::Log(Log, "Exiting engine");
}
