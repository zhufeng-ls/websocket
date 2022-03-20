
#ifndef JUMP_CORE_BASE_LOGGING_H
#define JUMP_CORE_BASE_LOGGING_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#ifndef SPDLOG_TRACE_ON
#define SPDLOG_TRACE_ON
#endif

#ifndef SPDLOG_DEBUG_ON
#define SPDLOG_DEBUG_ON
#endif



namespace jump
{
namespace log
{

extern std::shared_ptr<spdlog::logger> m_logger;
extern bool is_save_log_file;

void initLogger();
void shutdownLogger();

} // log
} // jump

#define LOG_TRACE(msg, ...) SPDLOG_LOGGER_TRACE(jump::log::m_logger,msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) SPDLOG_LOGGER_DEBUG(jump::log::m_logger,msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) SPDLOG_LOGGER_INFO(jump::log::m_logger,msg, ##__VA_ARGS__ )
#define LOG_WARN(msg, ...) SPDLOG_LOGGER_WARN(jump::log::m_logger,msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) SPDLOG_LOGGER_ERROR(jump::log::m_logger,msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...) SPDLOG_LOGGER_FATAL(jump::log::m_logger,msg, ##__VA_ARGS__ )


#endif  // JUMP_CORE_BASE_LOGGING_H