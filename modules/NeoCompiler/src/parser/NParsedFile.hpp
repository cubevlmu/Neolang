#pragma once

#include <vector>

namespace neo {

    namespace ast {
        class ASTNode;
    }

    class NParsedFile 
    {
    public:
        NParsedFile();
        ~NParsedFile() = default;

        void debugOutput(class NDebugOutput&);
        void saveTo(const char* path);
        bool loadFrom(const char* path);

        void clearNodes();

    public:
        std::vector<ast::ASTNode*> Nodes;
    };
}