#pragma once

#include "Base.hpp"
#include <unordered_map>

namespace neo {

    class ASTTypeNode : public ASTNode
    {
    public:
        ASTTypeNode(std::string module, std::string type);
        ~ASTTypeNode() override;

    public:
        std::string moduleStr;
        std::string typeStr;
    };


    class ASTArrayType : public ASTTypeNode
    {
    public:
        ASTArrayType(std::string module, std::string typeStr, bool isReceiver, std::initializer_list<int> size);
        ~ASTArrayType() override;

    public:
        bool isReceiver;
        i32 dimenssion;
        std::vector<i32> size;
    };


    class ASTPointerType : public ASTTypeNode
    {
    public:
        ASTPointerType(std::string module, std::string typeStr);
        ~ASTPointerType() override;
    };
}