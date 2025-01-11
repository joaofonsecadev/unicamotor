#pragma once

#include "core/vector.h"

namespace unc
{
class string
{
public:
    string();

    explicit string(const char* ptr) { ImportFromCharPtr(ptr); }
    [[nodiscard]] bool IsEmpty() const { return m_data.GetSize() <= 1; }

    char* GetData() { return m_data.GetData(); }

    string& operator=(const string& other) = default;
    string& operator=(const char* char_ptr);

private:
    void ImportFromCharPtr(const char* ptr);

    unc::vector<char> m_data;
};
}
