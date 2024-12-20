#pragma once

#include "profiling.h"

inline void* operator new(size_t size)
{
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
    TracyFree(pointer);
    free(pointer);
}

namespace unc
{
template <class T>
class unique_ptr
{
public:
    explicit unique_ptr(T* pointer = nullptr) : m_pointer(pointer) { }
    ~unique_ptr() { delete m_pointer; }

    T* operator->() const noexcept { return m_pointer; }

private:
    T* m_pointer;
};

template <class T, class... Args>
unique_ptr<T> unique_ptr_make(Args&&... args) { return unique_ptr<T>(new T(std::forward<Args>(args)...)); }
}
