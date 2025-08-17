// Created by cubevlmu on 2025/8/15.
// Copyright (c) 2025 Flybird Games. All rights reserved.

#pragma once

#include <neo/common.hpp>

#include <unordered_map>
#include <string>

namespace neo {

    /// Command line arguments parser
    /// --[FLAG][=VALUE] or -[FLAG][=VALUE] supporting boolean switch and string value
    class NCmdParser final
    {
        union CmdFlag {
            bool* bFlag;
            std::string* sFlag;
        };

    public:
        NCmdParser(i32 size, char** argv);
        ~NCmdParser();

        /// return false if any syntax error happend
        bool parse();

        /// register boolean switch receiver
        void regBool(const char* flag, bool*);
        /// register string value receiver
        void regStr(const char* flag, std::string&);

    private:
        std::unordered_map<std::string_view, CmdFlag> m_flags;
        std::string m_cmdLine;
    };

}