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
    Vector(const Vector& other) { operator=(other); }
    explicit Vector(size_t initial_capacity, const size_t max_capacity = 0)
    {
        UnicaProf_ZoneScoped;

        m_max_capacity = max_capacity;
        if (initial_capacity == 0)
        {
            return;
        }

        if (max_capacity && initial_capacity > max_capacity)
        {
            initial_capacity = max_capacity;
        }

        T* new_data_ptr = new T[initial_capacity];
        if (!new_data_ptr)
        {
            return;
        }

        m_data_pointer.Reset(new_data_ptr, true);
        m_capacity = initial_capacity;
    }

    ~Vector()
    {
        m_data_pointer.Reset(nullptr, true);
        m_capacity = 0;
        m_max_capacity = 0;
        m_size = 0;
    }

    T* PushBack(T new_element)
    {
        UnicaProf_ZoneScoped;
        if (m_size == m_capacity)
        {
            if (m_max_capacity > 0 && m_size == m_max_capacity)
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

    void CopyFromRawString(const char* src, const size_t size)
    {
        UnicaProf_ZoneScoped;
        if (src == nullptr || src == m_data_pointer.Get() || size == 0)
        {
            return;
        }

        const size_t elements_amount = size / sizeof(T);
        if (m_capacity < elements_amount)
        {
            Resize(elements_amount);
        }

        memcpy(m_data_pointer.Get(), src, size);
        m_size = elements_amount;
    }

    T* GetData() { return m_data_pointer.Get(); }
    [[nodiscard]] size_t GetSize() const { return m_size; }
    [[nodiscard]] size_t GetCapacity() const { return m_capacity; }

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

        if (m_max_capacity > 0 && new_capacity > m_max_capacity)
        {
            new_capacity = m_max_capacity;
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
    size_t m_max_capacity = 0;
};
}
