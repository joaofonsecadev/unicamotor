#include "core/string.h"

#include "core/profiling.h"

void Unica::String::DefaultDataVectorInitialization()
{
    UnicaProf_ZoneScoped;
    m_data = Unica::Vector<char>(1);
    m_data.PushBack('\0');
}

void Unica::String::ImportFromCharPtr(const char* ptr)
{
    UnicaProf_ZoneScoped;
    if (!ptr)
    {
        DefaultDataVectorInitialization();
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
        DefaultDataVectorInitialization();
        return;
    }

    m_data = Unica::Vector<char>(++string_size);
    m_data.CopyFromRawString(ptr, string_size * sizeof(char));
}

Unica::String& Unica::String::operator=(const String& other)
{
    UnicaProf_ZoneScoped;
    m_data = other.m_data;
    return *this;
}

Unica::String& Unica::String::operator=(const char* char_ptr)
{
    UnicaProf_ZoneScoped;
    ImportFromCharPtr(char_ptr);
    return *this;
}
