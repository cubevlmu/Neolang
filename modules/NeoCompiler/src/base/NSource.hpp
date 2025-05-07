#pragma once

#include <common.hpp>
#include <flib/types/FString.hpp>

namespace neo {

    class NSourceFile 
    {
    public:
        NSourceFile(const flib::FStringView& path, flib::IAllocator& alloc);
        ~NSourceFile();

        flib::Result readAll();
        flib::FStringView getContent() const;
        FLIB_FORCE_INLINE flib::FStringView getPath() const {
            return m_path;
        }
        std::string getFileName() const;

    private:
        flib::IAllocator& m_alloc;
        flib::FStringView m_path;
        std::string m_content;
    };
}