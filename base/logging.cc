#include "logging.h"

#include <unistd.h>
#include <assert.h>
#include <memory>

namespace jump
{
namespace log
{

std::shared_ptr<spdlog::logger> m_logger;
bool is_save_log_file = true;
std::string logfile = "logs/jump.log";

void initLogger() 
{
    if (spdlog::get("console") != nullptr) 
    {
        m_logger = spdlog::get("console");
        return;
    }

    std::vector<spdlog::sink_ptr> sinkList;
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level(spdlog::level::trace);
    sinkList.push_back(consoleSink);
    if (is_save_log_file)
    {
        spdlog::sink_ptr rotateSink = nullptr;
        rotateSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logfile, 1024 * 1024 * 10, 3);
        rotateSink->set_level(spdlog::level::info);
        sinkList.push_back(rotateSink);
    }
    m_logger = std::make_shared<spdlog::logger>("multi_sink",
        begin(sinkList), end(sinkList));
    m_logger->set_level(spdlog::level::trace);
    m_logger->flush_on(spdlog::level::debug);
    m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%^%l%$][thread %t, %s:%#, %!] %v");
}

void shutdownLogger() 
{
    spdlog::drop_all();
    spdlog::shutdown();
}

} // log
} // jump