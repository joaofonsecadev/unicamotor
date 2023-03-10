#pragma once

#include <string>
#include <vector>
#include <filesystem>

class UnicaSystemUtilities
{
public:
    static std::vector<char> ReadFileAsBytes(const std::string& UnicaFileLocation);

private:
    static std::filesystem::path ResolveUnicaFileLocation(const std::string& UnicaFileLocation);

};
