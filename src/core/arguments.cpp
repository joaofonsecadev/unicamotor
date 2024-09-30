#include "arguments.h"
#include "spdlog/spdlog.h"

void CommandLineParser::SetArgumentInput(int argc, char* argv[])
{
    m_argument_input.clear();
    m_argument_input.resize(argc - 1);

    for (int i = 1; i < argc; i++)
    {
        m_argument_input.at(i - 1) = argv[i];
    }
}

void CommandLineParser::Parse()
{
    for (int i = 0; i < m_argument_input.size(); i++)
    {
        if (m_argument_input.at(i).at(0) != '-')
        {
            continue;
        }

        auto short_argument = m_short_arguments.find(m_argument_input.at(i));
        if (short_argument != m_short_arguments.end())
        {
            if (i + 1 >= m_argument_input.size())
            {
                return;
            }

            if (m_argument_input.at(i + 1).at(0) == '-')
            {
                continue;
            }

            short_argument->second->value = m_argument_input.at(i + 1);
            short_argument->second->exists = true;
            i++;
            continue;
        }

        auto long_argument = m_long_arguments.find(m_argument_input.at(i));
        if (long_argument != m_long_arguments.end())
        {
            if (i + 1 >= m_argument_input.size())
            {
                return;
            }

            if (m_argument_input.at(i + 1).at(0) == '-')
            {
                continue;
            }

            long_argument->second.value = m_argument_input.at(i + 1);
            long_argument->second.exists = true;
            i++;
            continue;
        }
    }
}

std::string* CommandLineParser::GetArgumentValue(const std::string& argument_long_name)
{
    auto found_short_argument = m_short_arguments.find(argument_long_name);
    if (found_short_argument != m_short_arguments.end())
    {
        return &found_short_argument->second->value;
    }

    auto found_long_argument = m_long_arguments.find(argument_long_name);
    if (found_long_argument != m_long_arguments.end())
    {
        return &found_long_argument->second.value;
    }

    return nullptr;
}

void CommandLineParser::AddArgumentToParse(CommandLineArgument argument)
{
    argument.exists = false;
    argument.value = "";

    std::pair<std::unordered_map<std::string, CommandLineArgument>::iterator, bool> inserted_value =
            m_long_arguments.insert(std::pair<std::string, CommandLineArgument>(argument.long_name, argument));

    m_short_arguments.insert(std::pair<std::string, CommandLineArgument*>(argument.short_name, &inserted_value.first->second));
}

UnicamotorNetworkMode CommandLineParser::ResolveServerArgument(std::string* server_argument)
{
    if (server_argument == nullptr)
    {
        SPDLOG_ERROR("Provided server argument is nullptr, defaulting to Standalone network mode");
        return Standalone;
    }

    if (*server_argument == "server")
    {
        return Server;
    }
    else if (*server_argument == "standalone")
    {
        return Standalone;
    }
    else if (*server_argument == "client")
    {
        return Client;
    }

    SPDLOG_WARN("Failed to resolve '{}' to a valid network mode, defaulting to Standalone", *server_argument);
    return Standalone;
}
