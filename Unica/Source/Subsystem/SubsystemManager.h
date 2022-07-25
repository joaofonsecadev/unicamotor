// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <stack>
#include <memory>
// #include <type_traits>

#include "SubsystemBase.h"

class SubsystemManager
{
public:
    void Init();
    void Shutdown();

    /*
    template <typename T, std::enable_if_t<std::is_base_of_v<SubsystemBase, T>, std::nullptr_t> = nullptr>
    static T* GetSubsystem()
    {
        std::stack<std::unique_ptr<SubsystemBase>> SubsystemCollection = m_SubsystemCollection;
        while (!SubsystemCollection.empty())
        {
            T* Subsystem = static_cast<T*>(SubsystemCollection.top().get());
            if (Subsystem != nullptr)
            {
                return Subsystem;
            }
            SubsystemCollection.pop();
        }
        return nullptr;
    }*/
    
private:
    void InitializeSubsystem(SubsystemBase* Subsystem);
    
    static std::stack<std::unique_ptr<SubsystemBase>> m_SubsystemCollection;
};
