// Copyright João Fonseca, All Rights Reserved.

#include "UnicaPch.h"
#include "Renderer.h"

void Renderer::Init(UnicaEngine* const EngineInstance)
{
    EngineCoreComponent::Init(EngineInstance);
    
    UNICA_LOG(Log, "Initiating renderer...");
}
