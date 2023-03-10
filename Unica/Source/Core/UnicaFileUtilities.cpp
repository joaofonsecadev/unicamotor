#include "UnicaFileUtilities.h"

#include <fstream>

#include "UnicaInstance.h"
#include "Log/Logger.h"

std::filesystem::path UnicaFileUtilities::ResolveUnicaDirectory(const std::string& UnicaFileLocation)
{
    std::filesystem::path BaseDirectory = UnicaInstance::GetUnicaRootDirectory();
    
    std::filesystem::path UnicaFilePath(UnicaFileLocation);
    UnicaFilePath.make_preferred();
    
    if (UnicaFilePath.is_absolute())
    {
        return UnicaFilePath;
    }
    
    return BaseDirectory.append(UnicaFilePath.c_str());
}

std::vector<std::filesystem::path> UnicaFileUtilities::GetFilesInPathWithExtension(const std::string& PathToSearchString, const std::string& FileExtensionString)
{
    std::vector<std::string> FileExtension(1);
    FileExtension[0] = FileExtensionString;
    return GetFilesInPathWithExtension(PathToSearchString, FileExtension);
}

std::vector<std::filesystem::path> UnicaFileUtilities::GetFilesInPathWithExtension(const std::string& PathToSearchString, const std::vector<std::string>& FileExtensions)
{
    std::vector<std::filesystem::path> FinalFilesVector;
    const std::filesystem::path PathToSearch = ResolveUnicaDirectory(PathToSearchString);

    if (!std::filesystem::is_directory(PathToSearch))
    {
        const std::string ErrorMessage = std::format("Path '{}' is not valid", PathToSearch.string());
        UNICA_LOG(Error, __FUNCTION__, ErrorMessage);
        return FinalFilesVector;
    }
    
    for (const std::filesystem::directory_entry& PathEntry : std::filesystem::recursive_directory_iterator(PathToSearch))
    {
        if (PathEntry.is_directory())
        {
            continue;
        }

        std::filesystem::path PathEntryExtension = PathEntry.path().extension();
        for (const std::string& FileExtension : FileExtensions)
        {
            if (PathEntryExtension == FileExtension)
            {
                FinalFilesVector.push_back(PathEntry);
                break;
            }
        }
    }
    
    return FinalFilesVector;
}

std::vector<char> UnicaFileUtilities::ReadFileAsBinary(const std::string& UnicaFileLocation)
{
    const std::filesystem::path FileDirectory = ResolveUnicaDirectory(UnicaFileLocation);
    std::ifstream FileAsBinary(FileDirectory.c_str(), std::ios::ate, std::ios::binary);

    const std::streamsize FileSize = FileAsBinary.tellg();
    std::vector<char> Buffer(FileSize);

    FileAsBinary.seekg(0);
    FileAsBinary.read(Buffer.data(), FileSize);
    
    return Buffer;
}

std::string UnicaFileUtilities::ReadFileAsString(const std::string& UnicaFileLocation)
{
    const std::filesystem::path FileDirectory = ResolveUnicaDirectory(UnicaFileLocation);
    std::ifstream FileAsString(UnicaFileLocation);

    if (!FileAsString.is_open())
    {
        UNICA_LOG(Error, __FUNCTION__, std::format("Can't open file '{}'", FileDirectory.string()));
        return "";
    }

    std::stringstream StringBuffer;
    StringBuffer << FileAsString.rdbuf();
    FileAsString.close();
    return StringBuffer.str();
}
