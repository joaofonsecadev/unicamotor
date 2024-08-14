#include "tracy/Tracy.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "core/arguments.h"
#include "core/unicamotor.h"
#include "core/version.h"

void* operator new(std::size_t size)
{
    void* pointer = malloc(size);
    TracyAlloc(pointer, size);
    return pointer;
}

void operator delete(void* ptr) noexcept
{
    TracyFree(ptr);
    free(ptr);
}

void CreateLogger()
{
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/unicamotor.log", true);
    std::vector<spdlog::sink_ptr> logging_sinks { stdout_sink, file_sink };
    std::shared_ptr<spdlog::async_logger> logger = std::make_shared<spdlog::async_logger>("default_logger",logging_sinks.begin(),logging_sinks.end(),spdlog::thread_pool(),spdlog::async_overflow_policy::block);
    logger->set_pattern("[%T.%e] %-37!! [%^%5!l%$]: %v");
    spdlog::set_default_logger(logger);
    spdlog::flush_every(std::chrono::seconds(5));
}

void DefaultCommandLineParsing(int argc, char* argv[])
{
    CommandLineParser& command_line_parser = CommandLineParser::Get();
    command_line_parser.SetArgumentInput(argc, argv);
    command_line_parser.AddArgumentToParse({ "-n", "--network-mode" });
    command_line_parser.AddArgumentToParse({ "-t", "--target-frame-time-ms" });

    command_line_parser.Parse();

    if (argc <= 1)
    {
        return;
    }

    std::string all_arguments;
    for (int i = 1; i < argc; ++i)
    {
        all_arguments.append(argv[i]);
        all_arguments.append(" ");
    }

    SPDLOG_INFO("Commandline arguments: {}", all_arguments);
}

int main(int argc, char* argv[])
{
    CreateLogger();
    SPDLOG_INFO("Starting Unicamotor - v{}", VERSION_STRING);

    DefaultCommandLineParsing(argc, argv);

    CommandLineParser& command_line_parser = CommandLineParser::Get();
    std::string* network_mode_argument = command_line_parser.GetArgumentValue("--network-mode");
    UnicamotorNetworkMode network_mode = Standalone;
    if (network_mode_argument != nullptr && !network_mode_argument->empty())
    {
        network_mode = CommandLineParser::ResolveServerArgument(network_mode_argument);
    }

    Unicamotor engine(network_mode);
    while (!Unicamotor::HasExitBeenRequested())
    {
        engine.Tick();
        FrameMarkNamed("EngineLoop");
    }

    return 0;
}
