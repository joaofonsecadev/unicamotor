#pragma once

#include <string>
#include <vector>
#include <filesystem>

class UnicaFileUtilities
{
public:
    /**
     * Recursively get a vector with all of the files in a path with a specific file extension
     * @param PathToSearchString The relative Unica path from where to start the search. Must have a trailing slash
     * @param FileExtensionString The file extension, with the preceding dot (e.g, '.jpg')
     */
    static std::vector<std::filesystem::path> GetFilesInPathWithExtension(const std::string& PathToSearchString, const std::string& FileExtensionString);
    static std::vector<std::filesystem::path> GetFilesInPathWithExtension(const std::string& PathToSearchString, const std::vector<std::string>& FileExtensions);
    
    static std::vector<char> ReadFileAsBytes(const std::string& UnicaFileLocation);

private:
    static std::filesystem::path ResolveUnicaDirectory(const std::string& UnicaFileLocation);

};
