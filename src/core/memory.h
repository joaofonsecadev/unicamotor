#pragma once

#include "profiling.h"

inline void* operator new(size_t size)
{
    UnicaProf_ZoneScoped;
    void* pointer = malloc(size);
    if (!pointer)
    {
        throw std::bad_alloc();
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

namespace unc
{
template <class T>
class unique_ptr
{
public:
    unique_ptr() : m_pointer(nullptr) { }
    explicit unique_ptr(T* pointer = nullptr) : m_pointer(pointer) { }
    unique_ptr(unique_ptr&& other) noexcept : m_pointer(other.m_pointer) { other.m_pointer = nullptr; }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    ~unique_ptr() { Reset(nullptr); }

    T* Get() const { return m_pointer; }

    T* Release()
    {
        UnicaProf_ZoneScoped;
        T* pointer = m_pointer;
        m_pointer = nullptr;
        return pointer;
    }

    void Reset(T* new_pointer = nullptr)
    {
        UnicaProf_ZoneScoped;
        T* old_pointer = m_pointer;
        m_pointer = new_pointer;
        if (!old_pointer)
        {
            return;
        }
        delete old_pointer;
    }

    void Swap(unique_ptr& other) noexcept
    {
        UnicaProf_ZoneScoped;
        T* temp = static_cast<T*&&>(other.m_pointer);
        other.m_pointer = static_cast<T*&&>(m_pointer);
        m_pointer = static_cast<T*&&>(temp);
    }

    T& operator*() const { return *m_pointer; }
    T* operator->() const noexcept { return m_pointer; }
    explicit operator bool() const noexcept { return m_pointer != nullptr; }
    unique_ptr& operator=(unique_ptr&& other) noexcept
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
    T* m_pointer;
};

template <class T, class... Args>
unique_ptr<T> unique_ptr_make(Args&&... args) { return unique_ptr<T>(new T(static_cast<Args&&>(args)...)); }
}
