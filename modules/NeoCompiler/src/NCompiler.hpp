#pragma once

#include <flib/utils/SimpleCmdLineParser.hpp>
#include <flib/object/DefaultAllocator.hpp>

namespace neo {

    class NCompiler 
    {
    public:
        NCompiler(int argc, char** argv);
        ~NCompiler();

        int runCompiler();

    private:
        flib::FString m_args;
        flib::FCmdLineParser m_parser;
        flib::DefaultAllocator m_alloc;
    };
}