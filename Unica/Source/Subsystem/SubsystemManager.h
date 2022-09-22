// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include "SubsystemBase.h"

class SubsystemManager
{
public:
    void Init();
    void Shutdown();
    
    template <typename T, std::enable_if_t<std::is_base_of_v<SubsystemBase, T>, std::nullptr_t> = nullptr>
    static T* GetSubsystem()
    {
        for (const std::unique_ptr<SubsystemBase>& Subsystem : m_SubsystemCollection)
        {
            const T* const CastedSubsystem = static_cast<T*>(Subsystem.get());
            if (CastedSubsystem != nullptr)
            {
                return CastedSubsystem;
            }
        }
        return nullptr;
    }
    
private:
    void InitializeSubsystem(SubsystemBase* Subsystem);
    
    static std::vector<std::unique_ptr<SubsystemBase>> m_SubsystemCollection;
};
