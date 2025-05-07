#include "NDebugOutput.hpp"

#include <filesystem>

namespace neo {

    NFileOutput::NFileOutput(const flib::FStringView& path)
    {
        if (std::filesystem::exists(path.cstr())) {
            std::filesystem::remove(path.cstr());
        }

        m_fs.open(path.cstr());
        if (!m_fs.is_open()) {
            LogError("Failed to open file");
        }
    }


    NFileOutput::~NFileOutput()
    {
        m_fs.close();
    }

    void NFileOutput::writeLine(const flib::FStringView& line) {
        m_fs << line.cstr() << '\n';
    }

    void NFileOutput::write(const flib::FStringView& line) {
        m_fs << line.cstr();
    }

    bool NFileOutput::print() {
        m_fs.flags();
        return true;
    }
}