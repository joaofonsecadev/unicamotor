#pragma once

#include "profiling.h"

namespace unc
{
template <typename T>
class vector
{
public:
    vector() = default;
    ~vector()
    {
        UnicaProf_ZoneScoped;
        if (!m_data_pointer)
        {
            return;
        }

        TracyFree(m_data_pointer);
        free(m_data_pointer);
    }

    explicit vector(size_t initial_size)
    {
        UnicaProf_ZoneScoped;
        m_data_pointer = malloc(initial_size * sizeof(T));
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

    [[nodiscard]] size_t GetSize() const { return m_size; }

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