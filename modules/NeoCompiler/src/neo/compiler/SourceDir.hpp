// Created by cubevlmu on 2025/8/16.
// Copyright (c) 2025 Flybird Games. All rights reserved.

#pragma once

#include <unordered_map>
#include <string>

#include <neo/compiler/SourceFile.hpp>

namespace neo {

    class NSourceDir
    {
    public:
        NSourceDir(const char* path);
        ~NSourceDir();

        bool collect();
        bool compile();

        std::string_view getRoot() {
            return m_path;
        }

    private:
        std::unordered_map<std::string_view, NSourceFile> m_sources;
        std::string_view m_path;
    };
}