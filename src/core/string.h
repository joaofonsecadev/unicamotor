#pragma once

#include "core/profiling.h"
#include "core/vector.h"

namespace unc
{
class string
{
public:
    string()
    {
        UnicaProf_ZoneScoped;
        m_data = unc::vector<char>(1);
        m_data[0] = '\0';
    }

    explicit string(const char* ptr) { ImportFromCharPtr(ptr); }
    [[nodiscard]] bool IsEmpty() const { return m_data.GetSize() <= 1; }

    string& operator=(const string& other) = default;
    string& operator=(const char* char_ptr)
    {
        ImportFromCharPtr(char_ptr);
        return *this;
    }

private:
    void ImportFromCharPtr(const char* ptr)
    {
        UnicaProf_ZoneScoped;
        if (!ptr)
        {
            m_data = unc::vector<char>(1);
            m_data[0] = '\0';
            return;
        }

        size_t string_size = 0;
        const char* ptr_c = ptr;
        while (*ptr_c)
        {
            ++string_size;
            ++ptr_c;
        }

        if (string_size == 0)
        {
            m_data = unc::vector<char>(1);
            m_data[0] = '\0';
            return;
        }

        m_data = unc::vector<char>(++string_size);
        m_data.CopyFrom(ptr, string_size * sizeof(char));
    }

    unc::vector<char> m_data;
};
}
