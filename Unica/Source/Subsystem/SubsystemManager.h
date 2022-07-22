// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <stack>
#include <type_traits>

#include "SubsystemBase.h"

class SubsystemBase;

class SubsystemManager
{
public:
    void Init();

    template <typename T, std::enable_if_t<std::is_base_of_v<SubsystemBase, T>, std::nullptr_t> = nullptr>
    static T* GetSubsystem()
    {
        return nullptr;
    }
    
private:
    std::stack<SubsystemBase*> m_SubsystemCollection;
};
