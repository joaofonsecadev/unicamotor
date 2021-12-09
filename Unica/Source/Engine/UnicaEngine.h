// Copyright João Fonseca, All Rights Reserved.

#pragma once

class UnicaEngine
{
public:
    UnicaEngine() { }
    void Init();
    void Tick();
    void Exit();
    
    bool GetRequestedExit() const { return m_bRequestedExit; }

private:
    bool m_bRequestedExit = false;
};
