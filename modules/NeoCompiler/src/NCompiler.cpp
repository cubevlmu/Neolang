#include "NCompiler.hpp"

#include <sstream>

#include "base/NDebugOutput.hpp"
#include "base/NSource.hpp"
#include "lexer/NLexer.hpp"
#include "base/StringUtils.hpp"
#include "parser/NParsedFile.hpp"
#include "parser/NParser.hpp"

// #include "ast/ASTType.hpp"
// #include "ast/Decl.hpp"

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

        // ast::ModuleDecl def {"test"};
        // ast::ClassDecl cDecl {"TestClass", {}, &def};

        // ast::TypeCache cache {};
        // if (!cache.newType(&def, &cDecl)) {
        //     LogError("Insert failed!");
        //     return false;
        // }

        // auto stringTypeRef = cache.getType("string");

        // return false;

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

        NParsedFile pFile {};

        NParserArgs args {
            .file = &file,
            .langVer = 1,
            .lexer = &lex,
            .output = pFile
        };
        NParser parser {args};
        if (!parser.parse()) {
            LogError("Parsed failed");
        }

        return 0;
    }



}