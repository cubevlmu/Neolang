#pragma once

#include <common.hpp>
#include <fstream>
#include <sstream>

namespace neo {

    class NDebugOutput
    {
    public:
        virtual ~NDebugOutput() = default;

        virtual void writeLine(const flib::FStringView& line) = 0;
        virtual void write(const flib::FStringView& txt) = 0;
        virtual bool print() = 0;
    };


    class NConsoleOutput : public NDebugOutput
    {
    public:
        virtual ~NConsoleOutput() = default;

        virtual void writeLine(const flib::FStringView& line) override {
            m_ss << line.cstr();
            LogInfo(m_ss.str());
            
            m_ss.clear();
            m_ss.str("");
        }
        virtual void write(const flib::FStringView& txt) override {
            m_ss << txt.cstr();
        }
        virtual bool print() override { return true; }

    private:
        std::stringstream m_ss;
    };


    class NFileOutput : public NDebugOutput
    {
    public:
        NFileOutput(const flib::FStringView& path);
        virtual ~NFileOutput();

        virtual void writeLine(const flib::FStringView& line) override;
        virtual void write(const flib::FStringView& txt) override;
        virtual bool print() override;

    private:
        std::ofstream m_fs;
    };
}