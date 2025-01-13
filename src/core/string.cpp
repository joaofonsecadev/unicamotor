#include "core/string.h"

#include "core/profiling.h"

Unica::String::String()
{
    UnicaProf_ZoneScoped;
    m_data = Unica::Vector<char>(1);
    m_data[0] = '\0';
}

void Unica::String::ImportFromCharPtr(const char* ptr)
{
    UnicaProf_ZoneScoped;
    if (!ptr)
    {
        m_data = Unica::Vector<char>(1);
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
        m_data = Unica::Vector<char>(1);
        m_data[0] = '\0';
        return;
    }

    m_data = Unica::Vector<char>(++string_size);
    m_data.CopyFrom(ptr, string_size * sizeof(char));
}

Unica::String& Unica::String::operator=(const char* char_ptr)
{
    UnicaProf_ZoneScoped;
    ImportFromCharPtr(char_ptr);
    return *this;
}
