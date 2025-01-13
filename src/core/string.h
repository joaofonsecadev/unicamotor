#pragma once

#include "core/vector.h"

namespace Unica
{
class String
{
public:
    String();
    ~String() = default;

    explicit String(const char* ptr) { ImportFromCharPtr(ptr); }
    [[nodiscard]] bool IsEmpty() const { return m_data.GetSize() <= 1; }

    char* GetData() { return m_data.GetData(); }

    String& operator=(const String& other);
    String& operator=(const char* char_ptr);

private:
    void ImportFromCharPtr(const char* ptr);

    Unica::Vector<char> m_data;
};
}
