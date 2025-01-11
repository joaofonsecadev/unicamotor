#include "core/string.h"

#include "core/profiling.h"

unc::string::string()
{
    UnicaProf_ZoneScoped;
    m_data = unc::vector<char>(1);
    m_data[0] = '\0';
}

void unc::string::ImportFromCharPtr(const char* ptr)
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

unc::string& unc::string::operator=(const char* char_ptr)
{
    ImportFromCharPtr(char_ptr);
    return *this;
}
