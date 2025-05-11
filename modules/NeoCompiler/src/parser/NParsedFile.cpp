#include "NParsedFile.hpp"

#include <ast/ASTBase.hpp>

namespace neo {

    NParsedFile::NParsedFile() {

    }


    void NParsedFile::clearNodes() {
        for (auto* item : Nodes) {
            delete item;
        }
        Nodes.clear();
    }
    
} // namespace neo