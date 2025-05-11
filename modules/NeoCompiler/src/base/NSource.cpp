#include "NSource.hpp"

#include <fstream>
#include <sstream>

namespace neo {

    NSourceFile::NSourceFile(const std::string_view& path, flib::IAllocator& alloc)
        : m_alloc(alloc)
        , m_path {path}
        , m_content {}
    {
    }


    NSourceFile::~NSourceFile()
    {
        m_content.clear();
    }


    flib::Result NSourceFile::readAll()
    {
        std::ifstream stm {};
        stm.open(m_path.data());
        if (!stm.is_open()) {
            return flib::Result::kFileAccess;
        }
        
        std::string buf {};
        std::stringstream builder {};
        while (std::getline(stm, buf)) {
            builder << buf << '\n';
        }
        buf.clear();
        m_content = builder.str();

        return flib::Result::kNone;
    }


    std::string_view NSourceFile::getContent() const
    {
        if (m_content.length() <= 0) {
            return "";
        }
        return m_content.c_str();
    }


    std::string NSourceFile::getFileName() const
    {
        size_t idx = m_path.length();
        const char* ptr = m_path.end();

        while (idx > 0) {
            ptr--;
            idx--;
            if (*ptr == '/' || *ptr == '\\') 
                break;
        }
        auto len = m_path.length() - idx;

        char name[len+1];
        strcpy(name, ptr);
        name[len] = '\0';
        return {name};
    }


}