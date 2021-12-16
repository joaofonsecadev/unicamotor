// Copyright João Fonseca, All Rights Reserved.

#pragma once

class UnicaEngine;

class EngineComponent
{
public:
    virtual ~EngineComponent() { }
    virtual void Init(UnicaEngine* EngineInstance) { m_EngineInstance = EngineInstance; }
    virtual void Tick() = 0;

    UnicaEngine* GetEngineInstance() const { return m_EngineInstance; }

private:
    UnicaEngine* m_EngineInstance = nullptr;
};
