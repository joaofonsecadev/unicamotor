#pragma once

#include "profiling.h"

namespace Unica
{
template <typename T>
class Vector
{
public:
    Vector() = default;
    ~Vector()
    {
        UnicaProf_ZoneScoped;
        if (!m_data_pointer)
        {
            return;
        }

        TracyFree(m_data_pointer);
        free(m_data_pointer);
        m_data_pointer = nullptr;
        m_capacity = 0;
        m_size = 0;
    }

    explicit Vector(size_t initial_size)
    {
        UnicaProf_ZoneScoped;
        m_data_pointer = static_cast<T*>(malloc(initial_size * sizeof(T)));
        m_capacity = initial_size;
        TracyAlloc(m_data_pointer, initial_size * sizeof(T));
    }

    T* PushBack(T new_element)
    {
        UnicaProf_ZoneScoped;
        if (m_size == m_capacity)
        {
            const size_t new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
            if (!Resize(new_capacity))
            {
                return nullptr;
            }
        }

        m_data_pointer[m_size] = new_element;
        return &m_data_pointer[m_size++];
    }

    void CopyFrom(const void* src, const size_t size)
    {
        UnicaProf_ZoneScoped;
        if (src == nullptr || src == m_data_pointer || size == 0)
        {
            return;
        }

        if (m_capacity < size / sizeof(T))
        {
            Resize(size / sizeof(T));
        }

        memcpy(m_data_pointer, src, size);
        m_size = size / sizeof(T);
    }

    T* GetData() { return m_data_pointer; }
    [[nodiscard]] size_t GetSize() const { return m_size; }

    T& operator[](size_t index) { return m_data_pointer[index]; }
    const T& operator[](size_t index) const { return m_data_pointer[index]; }

    Vector<T>& operator=(const Vector<T>& other)
    {
        UnicaProf_ZoneScoped;
        if (this == &other)
        {
            return *this;
        }

        if (m_data_pointer)
        {
            TracyFree(m_data_pointer);
            free(m_data_pointer);

            m_data_pointer = nullptr;
            m_capacity = 0;
            m_size = 0;
        }

        m_data_pointer = static_cast<T*>(malloc(other.m_capacity * sizeof(T)));
        if (!m_data_pointer)
        {
            return *this;
        }

        TracyAlloc(m_data_pointer, m_capacity * sizeof(T));
        memcpy(m_data_pointer, other.m_data_pointer, other.m_size * sizeof(T));
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        return *this;
    }

private:
    bool Resize(size_t new_capacity)
    {
        UnicaProf_ZoneScoped;
        if (new_capacity <= m_capacity)
        {
            return false;
        }

        T* new_data_pointer = static_cast<T*>(realloc(m_data_pointer, new_capacity * sizeof(T)));
        if (!new_data_pointer)
        {
            return false;
        }

        TracyFree(m_data_pointer);
        TracyAlloc(new_data_pointer, new_capacity * sizeof(T));
        m_data_pointer = new_data_pointer;
        m_capacity = new_capacity;
        return true;
    }

    T* m_data_pointer = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;
};
}
