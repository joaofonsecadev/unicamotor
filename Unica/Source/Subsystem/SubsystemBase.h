// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

class SubsystemBase
{
public:
    virtual SubsystemBase* Init() = 0;
    virtual void Shutdown() = 0;
};