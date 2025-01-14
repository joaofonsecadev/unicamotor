#pragma once

#include "memory.h"
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
        m_data_pointer.Reset(nullptr, true);
        m_capacity = 0;
        m_max_size = 0;
        m_size = 0;
    }

    explicit Vector(size_t initial_size, size_t max_size = 0)
    {
        UnicaProf_ZoneScoped;

        m_max_size = max_size;
        if (initial_size == 0)
        {
            return;
        }

        if (max_size && initial_size > max_size)
        {
            initial_size = max_size;
        }

        T* new_data_ptr = new T[initial_size];
        if (!new_data_ptr)
        {
            return;
        }

        m_data_pointer.Reset(new_data_ptr, true);
        m_capacity = initial_size;
    }

    T* PushBack(T& new_element)
    {
        UnicaProf_ZoneScoped;
        if (m_size == m_capacity)
        {
            if (m_max_size > 0 && m_size == m_max_size)
            {
                return nullptr;
            }

            const size_t new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
            if (!Resize(new_capacity))
            {
                return nullptr;
            }
        }

        m_data_pointer.Get()[m_size] = new_element;
        return &m_data_pointer.Get()[m_size++];
    }

    void CopyFrom(const void* src, const size_t size)
    {
        UnicaProf_ZoneScoped;
        if (src == nullptr || src == m_data_pointer.Get() || size == 0)
        {
            return;
        }

        if (m_capacity < size / sizeof(T))
        {
            Resize(size / sizeof(T));
        }

        memcpy(m_data_pointer.Get(), src, size);
        m_size = size / sizeof(T);
    }

    T* GetData() { return m_data_pointer.Get(); }
    [[nodiscard]] size_t GetSize() const { return m_size; }

    T& operator[](size_t index) { return m_data_pointer.Get()[index]; }
    const T& operator[](size_t index) const { return m_data_pointer.Get()[index]; }

    Vector<T>& operator=(const Vector<T>& other)
    {
        UnicaProf_ZoneScoped;
        if (this == &other)
        {
            return *this;
        }

        T* m_new_data_pointer = new T[other.m_capacity];
        if (!m_new_data_pointer)
        {
            return *this;
        }
        m_data_pointer.Reset(m_new_data_pointer, true);
        memcpy(m_new_data_pointer, other.m_data_pointer.Get(), other.m_size * sizeof(T));
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        return *this;
    }

private:
    bool Resize(size_t new_capacity)
    {
        UnicaProf_ZoneScoped;

        if (m_max_size > 0 && new_capacity > m_max_size)
        {
            new_capacity = m_max_size;
        }

        if (new_capacity <= m_capacity)
        {
            return false;
        }

        T* new_data_pointer = new T[new_capacity];
        if (!new_data_pointer)
        {
            return false;
        }

        T* current_ptr = m_data_pointer.Get();
        for (uint32_t i = 0; i < m_size; i++)
        {
            new_data_pointer[i] = current_ptr[i];
        }

        m_data_pointer.Reset(new_data_pointer, true);
        m_capacity = new_capacity;
        return true;
    }

    Unica::UniquePtr<T> m_data_pointer;
    size_t m_size = 0;
    size_t m_capacity = 0;
    size_t m_max_size = 0;
};
}
