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

    string(const char* ptr)
    {
        UnicaProf_ZoneScoped;
        if (!ptr)
        {
            m_data = unc::vector<char>(1);
            m_data[0] = '\0';
            return;
        }

        size_t string_size = 0;
        for (uint32_t i = 0; ptr[i] != '\0'; i++)
        {
            string_size++;
        }

        m_data = unc::vector<char>(++string_size);
        for (size_t i = 0; i < string_size - 1; i++)
        {
            m_data.PushBack(ptr[i]);
        }
        m_data.PushBack('\0');
    }

private:
    unc::vector<char> m_data;
};
}
