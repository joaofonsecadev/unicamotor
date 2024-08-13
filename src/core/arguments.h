#pragma once

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

struct CommandLineArgument
{
    CommandLineArgument() = default;
    CommandLineArgument(std::string in_short_name, std::string in_long_name) : short_name(std::move(in_short_name)), long_name(std::move(in_long_name)) { }

    std::string short_name;
    std::string long_name;
    std::string value;
    bool exists = false;
};

class CommandLineParser
{
public:
    void SetArgumentInput(int argc, char* argv[]);
    static CommandLineParser& Get()
    {
        static CommandLineParser singleton;
        return singleton;
    }

    void Parse();
    std::string* GetArgumentValue(std::string& argument_long_name);
    void AddArgumentToParse(CommandLineArgument argument);

private:
    std::vector<std::string> m_argument_input;
    std::unordered_map<std::string, CommandLineArgument> m_long_arguments;
    std::unordered_map<std::string, CommandLineArgument*> m_short_arguments;
};
