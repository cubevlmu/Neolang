#pragma once

#include "common.hpp"
#include "neo/compiler/SourceDir.hpp"

#include <string>

namespace neo {

    class NCmdParser;

    struct CompilerConfig
    {
        std::string sourceDir;
    };


    class NCompiler 
    {
    public:
        NCompiler(int argc, char** argv);
        ~NCompiler();

        int runCompiler();

    private:
        static void regFlags(NCmdParser*);

    private:
        static CompilerConfig s_cfg;

        std::vector<NSourceDir> m_soruceDirs;
    };
}