#include "UnicaFileUtilities.h"

#include <fstream>

#include "UnicaInstance.h"
#include "Log/Logger.h"

std::filesystem::path UnicaFileUtilities::ResolveUnicaDirectory(const std::string& UnicaFileLocation)
{
    std::filesystem::path BaseDirectory = UnicaInstance::GetUnicaRootDirectory();
    
    std::filesystem::path UnicaFilePath(UnicaFileLocation);
    UnicaFilePath.make_preferred();
    
    return BaseDirectory.append(UnicaFilePath.c_str());
}

std::vector<std::filesystem::path> UnicaFileUtilities::GetFilesInPathWithExtension(const std::string& PathToSearchString, const std::string& FileExtensionString)
{
    std::vector<std::filesystem::path> FinalFilesVector;
    const std::filesystem::path PathToSearch = ResolveUnicaDirectory(PathToSearchString);

    if (!std::filesystem::is_directory(PathToSearch))
    {
        const std::string ErrorMessage = std::format("Path '{}' is not valid", PathToSearch.string());
        UNICA_LOG(Error, "LogUnicaFileUtilities", ErrorMessage);
        return FinalFilesVector;
    }
    
    for (const std::filesystem::directory_entry& PathEntry : std::filesystem::recursive_directory_iterator(PathToSearch))
    {
        if (PathEntry.is_directory())
        {
            continue;
        }

        if (PathEntry.path().extension() != FileExtensionString)
        {
            continue;
        }

        FinalFilesVector.push_back(PathEntry);
    }
    
    return FinalFilesVector;
}

std::vector<char> UnicaFileUtilities::ReadFileAsBytes(const std::string& UnicaFileLocation)
{
    const std::filesystem::path FileDirectory = ResolveUnicaDirectory(UnicaFileLocation);
    std::ifstream FileAsBytes(FileDirectory.c_str(), std::ios::ate, std::ios::binary);

    const std::streamsize FileSize = FileAsBytes.tellg();
    std::vector<char> Buffer(FileSize);

    FileAsBytes.seekg(0);
    FileAsBytes.read(Buffer.data(), FileSize);
    
    return Buffer;
}
