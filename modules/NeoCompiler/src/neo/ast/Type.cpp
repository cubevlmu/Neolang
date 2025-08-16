#include "Type.hpp"

namespace neo {

    ASTTypeNode::ASTTypeNode(std::string module, std::string type)
        : ASTNode(kType)
        , moduleStr {std::move(module) }
        , typeStr {std::move(type)}
    {

    }

    ASTTypeNode::~ASTTypeNode() {

    }
}