#include "NCompiler.hpp"

#include <sstream>

#include "base/NDebugOutput.hpp"
#include "base/NSource.hpp"
#include "lexer/NLexer.hpp"
#include "base/StringUtils.hpp"

namespace neo {

    flib::FString concatArgs(flib::IAllocator& alloc, int argc, char** argv) {
        flib::FString args { alloc };
        std::stringstream ss {};

        for (auto i = 1; i < argc; i++) {
            ss << args[i] << ' ';
        }
        args = args.sprintf("%s", ss.str().c_str());
        return args;
    }


    NCompiler::NCompiler(int argc, char** argv)
        : m_args {concatArgs(m_alloc, argc, argv)}
        , m_parser (m_args.cstr())
    {
    }


    NCompiler::~NCompiler()
    {
    }


    int NCompiler::runCompiler()
    {
        printf("Hello world\n");

        // test nsource
        NSourceFile file {"lex.neo", m_alloc};
        if (file.readAll()) {
            LogError("ERROR to read lex.neo");
            return -1;
        }

        NLexer lex {file, m_alloc};
        if (!lex.lex()) {
            LogError("Failed to lex file lex.neo");
            return -1;
        }

        auto name = file.getFileName();
        replaceAll(name, ".", "_");

        NFileOutput output {("lex_debug_" + name + ".txt").c_str()};
        lex.debugPrint(output);
        output.print();

        return 0;
    }



}