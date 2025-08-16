#pragma once

#include "neo/common.hpp"
#include <string>

namespace neo {

    class NSourceFile final
    {
    public:
        NSourceFile(class NSourceDir& dir, const std::string_view rPath);
        ~NSourceFile();

        bool readAll();
        std::string_view getContent() const;
        std::string getPath() const;
        std::string getFileName() const;

        bool compile();

    private:
        std::string_view m_rPath;
        std::string m_content;
        NSourceDir& m_dir;
    };

}