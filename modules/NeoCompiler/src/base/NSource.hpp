#pragma once

#include <common.hpp>
#include <flib/types/FString.hpp>

namespace neo {

    class NSourceFile 
    {
    public:
        NSourceFile(const std::string_view& path, flib::IAllocator& alloc);
        ~NSourceFile();

        flib::Result readAll();
        std::string_view getContent() const;
        FLIB_FORCE_INLINE std::string_view getPath() const {
            return m_path;
        }
        std::string getFileName() const;

    private:
        flib::IAllocator& m_alloc;
        std::string_view m_path;
        std::string m_content;
    };
}