#pragma once

#include "SourceLoc.hpp"
#include "base/NSource.hpp"
#include "lexer/Tokens.hpp"

namespace neo {

#include <type_traits>

    enum class DiagnosticLevel : u32 {
        kNone = 0,
        kError = 1 << 0,
        kWarning = 1 << 1,
        kNote = 1 << 2,
    };


    struct Diagnostic {
        DiagnosticLevel level;
        SourceLoc location;
        std::string message;
    };


    class DiagnosticCollector
    {
    public:
        DiagnosticCollector() = default;

    public:
        void report(DiagnosticLevel level, const SourceLoc& loc, const std::string& message);

        FLIB_FORCE_INLINE void error(const SourceLoc& loc, const std::string& msg) {
            report(DiagnosticLevel::kError, loc, msg);
        }
        FLIB_FORCE_INLINE void warning(const SourceLoc& loc, const std::string& msg) {
            report(DiagnosticLevel::kWarning, loc, msg);
        }
        FLIB_FORCE_INLINE void note(const SourceLoc& loc, const std::string& msg) {
            report(DiagnosticLevel::kNote, loc, msg);
        }

        FLIB_FORCE_INLINE bool hasError() const { return m_errorCount > 0; }
        FLIB_FORCE_INLINE int getErrorCount() const { return m_errorCount; }
        const std::vector<Diagnostic>& diagnostics() const { return m_diagnostics; }

        void printAll() const;
        void clear(DiagnosticLevel flags = DiagnosticLevel::kNone);

    private:
        std::vector<Diagnostic> m_diagnostics;
        int m_errorCount = 0;
    };


    template<typename T>
    class ErrorOr {
    public:
        static ErrorOr<T> success(T value) {
            return ErrorOr(std::move(value));
        }

        static ErrorOr<T> failure(std::string errorMsg) {
            return ErrorOr(std::move(errorMsg));
        }

        bool hasError() const { return m_hasError; }
        const std::string& errorMessage() const { return m_errorMessage; }
        const T& value() const { FLIB_ASSERT(!m_hasError); return m_value; }
        T& value() { FLIB_ASSERT(!m_hasError); return m_value; }

        explicit operator bool() const { return !m_hasError; }

    private:
        ErrorOr(T val)
            : m_value(std::move(val)), m_hasError(false) {
        }

        ErrorOr(std::string msg)
            : m_errorMessage(std::move(msg)), m_hasError(true) {
        }

        T m_value;
        std::string m_errorMessage;
        bool m_hasError = true;
    };


    template<>
    class ErrorOr<void> {
    public:
        static ErrorOr<void> success() {
            return ErrorOr();
        }

        static ErrorOr<void> failure(std::string errorMsg) {
            return ErrorOr(std::move(errorMsg));
        }

        bool hasError() const { return m_hasError; }
        const std::string& errorMessage() const { return m_errorMessage; }

        explicit operator bool() const { return !m_hasError; }

    private:
        ErrorOr() : m_hasError(false) {}
        ErrorOr(std::string msg)
            : m_errorMessage(std::move(msg)), m_hasError(true) {
        }

        std::string m_errorMessage;
        bool m_hasError = true;
    };


    class Error
    {
    public:
        Error() = default;

        static Error success() { return Error(); }
        static Error failure(std::string msg) { return Error(std::move(msg)); }
        static Error failure(std::string msg, DiagnosticCollector* c, NToken& t, NSourceFile* f) { 
            c->error(t.location(f), msg);
            return Error(std::move(msg)); 
        }

        bool hasError() const { return !m_messages.empty(); }
        const std::vector<std::string>& messages() const { return m_messages; }

        void append(std::string msg) {
            m_messages.push_back(std::move(msg));
        }

        Error& operator<<(const std::string& msg) {
            append(msg);
            return *this;
        }

    private:
        std::vector<std::string> m_messages;

        explicit Error(std::string msg) {
            m_messages.push_back(std::move(msg));
        }
    };


    template<typename T>
    class Expected 
    {
    public:
        Expected(T value)
            : m_value(std::move(value)), m_hasError(false), m_got {false} {
        }
        Expected(Error error)
            : m_error(std::move(error)), m_hasError(true), m_got {false} {
        }
        ~Expected() {
            if (m_got) return;
            if (!m_value) return;
            delete m_value;
        }

        bool hasError() const { return m_hasError; }
        const Error& error() const { return m_error; }
        Error& error() { return m_error; }

        const T& value() const { FLIB_ASSERT(!m_hasError); return m_value; }
        T& value() { 
            FLIB_ASSERT(!m_hasError); 
            m_got = true;
            return m_value;
        }
        T& operator->() {
            return m_value;
        }

        explicit operator bool() const { return !m_hasError; }

    private:
        T m_value;
        Error m_error;
        bool m_hasError;
        bool m_got;
    };


    template<>
    class Expected<void> 
    {
    public:
        Expected() : m_hasError(false) {}
        Expected(Error error) : m_error(std::move(error)), m_hasError(true) {}

        bool hasError() const { return m_hasError; }
        const Error& error() const { return m_error; }
        Error& error() { return m_error; }

        explicit operator bool() const { return !m_hasError; }

    private:
        Error m_error;
        bool m_hasError;
    };

#define CHECK_ERROR(V) do { \
    if (!V) { \
        return V.error(); \
    } \
} while(false)


    template<typename E>
    constexpr auto to_underlying(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
    constexpr DiagnosticLevel operator|(DiagnosticLevel lhs, DiagnosticLevel rhs) {
        return static_cast<DiagnosticLevel>(to_underlying(lhs) | to_underlying(rhs));
    }
    constexpr DiagnosticLevel operator&(DiagnosticLevel lhs, DiagnosticLevel rhs) {
        return static_cast<DiagnosticLevel>(to_underlying(lhs) & to_underlying(rhs));
    }
    constexpr DiagnosticLevel operator~(DiagnosticLevel v) {
        return static_cast<DiagnosticLevel>(~to_underlying(v));
    }
    inline DiagnosticLevel& operator|=(DiagnosticLevel& lhs, DiagnosticLevel rhs) {
        lhs = lhs | rhs;
        return lhs;
    }
    inline DiagnosticLevel& operator&=(DiagnosticLevel& lhs, DiagnosticLevel rhs) {
        lhs = lhs & rhs;
        return lhs;
    }
}