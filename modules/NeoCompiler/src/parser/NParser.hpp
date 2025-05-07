#pragma once

#include <common.hpp>

namespace neo {

    struct NParserArgs
    {
        class NLexer* lexer;
        i32 langVer;
    };

    class NParser 
    {
    public:
        NParser(NParserArgs args, flib::IAllocator& alloc);
        ~NParser();

    public:
        bool parse();
        void debugPrint(class NDebugOutput&);

    private:
        flib::IAllocator& m_alloc;
        NParserArgs m_args;
    };
}