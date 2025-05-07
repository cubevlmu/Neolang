#pragma once

#include <flib/common.hpp>

#include <string>
#include <sstream>
#include <flib/types/FString.hpp>
#include <memory>

#ifdef LogInfo
#undef LogInfo
#endif
#ifdef LogError
#undef LogError
#endif
#ifdef LogWarn
#undef LogWarn
#endif
#ifdef LogDebug
#undef LogDebug
#endif

namespace spdlog {
    class logger;
}

namespace neo {

    enum LogLevel {
        LogWarn, LogError, LogDebug, LogInfo, LogTrace
    };

    class Logger final
    {
    private:
        FLIB_FORCE_INLINE void addLog(const char* val) { ss << val; }
        FLIB_FORCE_INLINE void addLog(const std::string_view& val) { ss << val; }
        FLIB_FORCE_INLINE void addLog(const std::string& val) { ss << val; }
        FLIB_FORCE_INLINE void addLog(std::uint64_t val) { ss << val; }
        FLIB_FORCE_INLINE void addLog(std::uint32_t val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(std::uint16_t val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(std::uint8_t val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(std::int64_t val) { ss << val; }
        FLIB_FORCE_INLINE void addLog(std::int32_t val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(std::int16_t val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(std::int8_t val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(bool val) {ss << (val ? "true" : "false");}
	    void addLog(const flib::FStringView& val) {
            char toPrint[val.length()];
            strcpy(toPrint, val.begin());
            toPrint[val.length()-1] = '\0';
            ss << toPrint;
        }

        FLIB_FORCE_INLINE void addLog(char val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(long val) {ss << val;}

        FLIB_FORCE_INLINE void addLog(float val) {ss << val;}
        FLIB_FORCE_INLINE void addLog(double val) {ss << val;}

        void emitLog(LogLevel level);

        template <typename... T> 
        FLIB_FORCE_INLINE void log(LogLevel level, const T&... args) {
            int tmp[] = { (addLog(args), 0) ... };
            (void)tmp;
            emitLog(level);
        }

    public:
        FLIB_FORCE_INLINE Logger(const char* name, bool useFileRecorder) {
            init(name, useFileRecorder);
        }

        void init(const char* name, bool useFileRecorder);

        template <typename... T> 
        FLIB_FORCE_INLINE void info(const T&... args) 
        { log(LogLevel::LogInfo, args...); }

        template <typename... T> 
        FLIB_FORCE_INLINE void warn(const T&... args) 
        { log(LogLevel::LogWarn, args...); }

        template <typename... T> 
        FLIB_FORCE_INLINE void error(const T&... args) 
        { log(LogLevel::LogError, args...); }

        template <typename... T> 
        FLIB_FORCE_INLINE void debug(const T&... args) 
        { log(LogLevel::LogDebug, args...); }

        template <typename... T> 
        FLIB_FORCE_INLINE void trace(const T&... args) 
        { log(LogLevel::LogTrace, args...); }

    private:
        std::stringstream ss;
        std::shared_ptr<spdlog::logger> m_backend;
    };

    Logger* getNeoDefaultLogger(const char* name = "neo", bool useFileRecorder = false);
}

#define LogInfo(...)  ::neo::getNeoDefaultLogger()->info(__VA_ARGS__)
#define LogWarn(...)  ::neo::getNeoDefaultLogger()->warn(__VA_ARGS__)
#define LogError(...) ::neo::getNeoDefaultLogger()->error(__VA_ARGS__)
#define LogDebug(...) ::neo::getNeoDefaultLogger()->debug(__VA_ARGS__)
#define LogTrace(...) ::neo::getNeoDefaultLogger()->trace(__VA_ARGS__)