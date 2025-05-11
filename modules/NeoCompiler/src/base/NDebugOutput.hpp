#pragma once

#include <common.hpp>
#include <fstream>
#include <sstream>

namespace neo {

    class NDebugOutput
    {
    public:
        virtual ~NDebugOutput() = default;

        virtual void writeLine(const std::string_view& line) = 0;
        virtual void write(const std::string_view& txt) = 0;
        virtual bool print() = 0;
    };


    class NConsoleOutput : public NDebugOutput
    {
    public:
        virtual ~NConsoleOutput() = default;

        virtual void writeLine(const std::string_view& line) override {
            m_ss << line.data();
            LogInfo(m_ss.str());

            m_ss.clear();
            m_ss.str("");
        }
        virtual void write(const std::string_view& txt) override {
            m_ss << txt.data();
        }
        virtual bool print() override { return true; }

    private:
        std::stringstream m_ss;
    };


    class NFileOutput : public NDebugOutput
    {
    public:
        NFileOutput(const std::string_view& path);
        virtual ~NFileOutput();

        virtual void writeLine(const std::string_view& line) override;
        virtual void write(const std::string_view& txt) override;
        virtual bool print() override;

    private:
        std::ofstream m_fs;
    };
}