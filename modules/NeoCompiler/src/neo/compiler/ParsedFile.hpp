#pragma once

#include <vector>

namespace neo {

    class ASTNode;\
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
        std::vector<ASTNode*> Nodes;
    };
}