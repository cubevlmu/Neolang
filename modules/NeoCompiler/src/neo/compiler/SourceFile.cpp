#include "SourceFile.hpp"

#include "neo/compiler/SourceDir.hpp"
#include "neo/compiler/Lexer.hpp"
#include "neo/compiler/Parser.hpp"
#include "neo/compiler/ParsedFile.hpp"
#include "neo/base/Logger.hpp"

#include <fstream>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

namespace neo {

    NSourceFile::NSourceFile(NSourceDir& s, const std::string_view rp)
        : m_rPath {rp}
        , m_content {}
        , m_dir {s}
    {
    }

    NSourceFile::~NSourceFile()
    {
        m_content.clear();
    }

    bool NSourceFile::readAll()
    {
        std::ifstream stm {};
        stm.open(m_rPath);
        if (!stm.is_open()) {
            LogError("Failed to read soruce file {}", m_rPath);
            return false;
        }
        
        std::string buf {};
        std::stringstream builder {};
        while (std::getline(stm, buf)) {
            builder << buf << '\n';
        }
        buf.clear();
        m_content = builder.str();

        return true;
    }

    std::string_view NSourceFile::getContent() const
    {
        if (m_content.empty()) {
            return "";
        }
        return m_content;
    }

    std::string NSourceFile::getFileName() const
    {
        fs::path p {m_rPath};
        return p.filename().string();
    }

    std::string NSourceFile::getPath() const {
        std::string str {};
        str.resize(m_dir.getRoot().length() + m_rPath.length() + 1);
        str.append(m_dir.getRoot());
        str.append("//");
        str.append(m_rPath);
        return str;
    }

    bool NSourceFile::compile() {
        NLexer lex {this};
        if (!lex.lex()) {
            LogError("Failed to lex file lex.neo");
            return -1;
        }

        NParsedFile file {};
        NParserArgs args {
            .lexer = &lex,
            .file = this,
            .output = file,
            .langVer = 1,
        };
        NParser parser {args};
        if (!parser.parse()) {
            return false;
        }

        // Symbol collect pass & analyzer pass

        return true;
    }

}
