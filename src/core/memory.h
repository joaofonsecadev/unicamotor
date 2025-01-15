#pragma once

#include "profiling.h"

inline void* operator new(size_t size)
{
    UnicaProf_ZoneScoped;
    void* pointer = malloc(size);
    if (!pointer)
    {
        return nullptr;
    }

    TracyAlloc(pointer, size);
    return pointer;
}

inline void operator delete(void* pointer)
{
    UnicaProf_ZoneScoped;
    TracyFree(pointer);
    free(pointer);
}

inline void* operator new[](size_t size)
{
    return operator new(size);
}

inline void operator delete[](void* pointer)
{
    UnicaProf_ZoneScoped;
    TracyFree(pointer);
    free(pointer);
}

namespace Unica
{
template <class T>
class UniquePtr
{
public:
    UniquePtr() = default;
    explicit UniquePtr(T* pointer) : m_pointer(pointer) { }
    UniquePtr(UniquePtr&& other) noexcept : m_pointer(other.m_pointer) { other.m_pointer = nullptr; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    ~UniquePtr() { Reset(nullptr); }

    T* Get() const { return m_pointer; }

    T* Release()
    {
        UnicaProf_ZoneScoped;
        T* pointer = m_pointer;
        m_pointer = nullptr;
        return pointer;
    }

    void Reset(T* new_pointer = nullptr, bool is_vec_ptr = false)
    {
        UnicaProf_ZoneScoped;
        T* old_pointer = m_pointer;
        m_pointer = new_pointer;
        if (!old_pointer)
        {
            return;
        }
        if (is_vec_ptr)
        {
            delete[] old_pointer;
            return;
        }
        delete old_pointer;
    }

    void Swap(UniquePtr& other) noexcept
    {
        UnicaProf_ZoneScoped;
        T* temp = other.m_pointer;
        other.m_pointer = m_pointer;
        m_pointer = temp;
    }

    T& operator*() const { return *m_pointer; }
    T* operator->() const noexcept { return m_pointer; }
    explicit operator bool() const noexcept { return m_pointer != nullptr; }
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        UnicaProf_ZoneScoped;
        if (this == &other)
        {
            return *this;
        }
        Reset(other.m_pointer);
        other.m_pointer = nullptr;
        return *this;
    }

private:
    T* m_pointer = nullptr;
};

template <class T, class... Args>
UniquePtr<T> CreateUniquePtr(Args&&... args) { return UniquePtr<T>(new T(static_cast<Args&&>(args)...)); }
}
