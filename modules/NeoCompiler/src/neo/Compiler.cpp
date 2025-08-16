#include "Compiler.hpp"

#include "neo/base/StringUtils.hpp"
#include "neo/base/CmdParser.hpp"
#include "neo/base/Timer.hpp"
#include "neo/base/Logger.hpp"

#include <iostream>

namespace neo {

    CompilerConfig NCompiler::s_cfg{
        .sourceDir = {}
    };

    NCompiler::NCompiler(int argc, char **argv) {
        NCmdParser cmdPs{argc, argv};
        NCompiler::regFlags(&cmdPs);
        if (!cmdPs.parse()) {
            std::cerr << "Invalid arguments! Failed to parse arguments\n";
            std::exit(-1);
        }
    }

    NCompiler::~NCompiler() {
    }

    void NCompiler::regFlags(neo::NCmdParser* p) {
        p->regStr("srcDir", s_cfg.sourceDir);
    }

    int NCompiler::runCompiler() {
        if (s_cfg.sourceDir.empty()) {
            LogError("No source dir input! Compiler halt.");
            return 0;
        }

        NTimer t{};
        std::vector<std::string> out {};
        splitStr(out, s_cfg.sourceDir, ';');

        for (auto& str : out) {
            NSourceDir dir {str.c_str()};
            m_soruceDirs.push_back(dir);
            if (!dir.collect()) {
                LogDebug("No source file in dir {}", str);
            }
        }

        bool r = false;
        for (auto& dir : m_soruceDirs) {
            r |= dir.compile();
        }

        t.end();
        if (!r) {
            LogError("Error occurrenced in compile process! Compiler halt in {} s.", t.secondTime());
        } else {
            LogInfo("Compiler process end in {} s.", t.secondTime());
        }

        return 0;
    }


}