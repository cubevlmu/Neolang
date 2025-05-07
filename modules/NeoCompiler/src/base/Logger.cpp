#include "Logger.hpp"

#include <memory>
#include <sstream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define FORMAT "[%^%L%$] %v"

namespace neo {

    void Logger::init(const char* name, bool useFileRecorder) {
        FastConcat(fileName, name, ".log");

        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks[0]->set_pattern(FORMAT);
        if (useFileRecorder) {
            logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>((const char*)fileName, true));
            logSinks[1]->set_pattern(FORMAT);
        }

        m_backend = std::make_shared<spdlog::logger>(name, begin(logSinks), end(logSinks));
        spdlog::register_logger(m_backend);
        m_backend->set_level(spdlog::level::trace);
        m_backend->flush_on(spdlog::level::trace);
    }


    void Logger::emitLog(LogLevel level) {
        auto stdstr = ss.str();
        const char* str = stdstr.c_str();
        //TODO Add Log Callback Action To Here
        ss.str("");
        ss.clear();


        switch (level)
        {
        case LogWarn:
            m_backend->warn(str); break;
        case LogError:
            m_backend->error(str); break;
        case LogDebug:
            m_backend->debug(str); break;
        case LogInfo:
            m_backend->info(str); break;
        case LogTrace:
            m_backend->trace(str); break;
        }
    }


    Logger* getNeoDefaultLogger(const char* name, bool useFileRecorder) {
        static Logger s_logger{name, useFileRecorder};
        return &s_logger;
    }
} // namespace neo