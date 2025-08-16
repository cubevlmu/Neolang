#pragma once

#include "Base.hpp"
#include "Type.hpp"

namespace neo {

    class VarDecl : public ASTDecl
    {
    public:
        VarDecl(const std::string_view& name, ASTTypeNode* type, ASTExpr* init = nullptr, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kVar)
            , name{ name }
            , parent{ parent }
            , type{ type }
            , initExpr{ init }
        {
        }

    public:
        ASTDecl* parent = nullptr;
        std::string name;
        ASTTypeNode* type;
        ASTExpr* initExpr = nullptr;
        bool isConst = false;
        bool isStatic = false;
    };


    class FuncDecl : public ASTDecl
    {
    public:
        FuncDecl(const std::string_view& name, ASTTypeNode* retType, std::vector<VarDecl*> args, class CompoundStmt* body = nullptr, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kFunc)
            , name{ name }
            , returnType{ retType }
            , args{ args }
            , funcBody{ body }
            , parent{ parent }
        {
        }
        ~FuncDecl() = default;

    public:
        std::string name;
        ASTTypeNode* returnType;
        std::vector<VarDecl*> args;
        CompoundStmt* funcBody;
        ASTDecl* parent = nullptr;

        bool isMarkedExtern;
        bool isMarkedInline;
        bool isClassFunc;
        bool isVirtualFunc;
        bool isOverrideFunc;
    };


    class FieldDecl : public ASTDecl
    {
    public:
        FieldDecl(const std::string_view& name, ASTTypeNode* type, ASTDecl* parent, ASTExpr* init = nullptr)
            : ASTDecl(DeclKind::kField)
            , name{ name }
            , parent{ parent }
            , type{ type }
            , init{ init }
        {
        }
        ~FieldDecl() = default;

    public:
        std::string name;
        ASTDecl* parent;
        ASTTypeNode* type;
        ASTExpr* init = nullptr;
    };


    class ClassDecl : public ASTDecl
    {
    public:
        ClassDecl(const std::string_view& name, std::vector<ASTTypeNode*> baseClasses, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kClass)
            , name{ name }
            , baseClasses{ baseClasses }
            , parent{ parent }
        {
        }
        ~ClassDecl() = default;

    public:
        std::string name;
        std::vector<ASTTypeNode*> baseClasses;
        std::vector<ASTDecl*> subClasses;
        std::vector<FieldDecl*> fields;
        std::vector<VarDecl*> variables;
        std::vector<FuncDecl*> functions;
        std::vector<FuncDecl*> ctors;
        FuncDecl* dtors;
        ASTDecl* parent;
    };


    class StructDecl : public ASTDecl
    {
    public:
        StructDecl(const std::string_view& name, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kStruct)
            , name{ name }
            , parent{ parent }
        {
        }
        ~StructDecl() = default;

    public:
        std::string name;
        std::vector<VarDecl*> variables;
        std::vector<FieldDecl*> fields;
        ASTDecl* parent = nullptr;
    };


    class InterfaceDecl : public ASTDecl
    {
    public:
        InterfaceDecl(const std::string_view& name, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kInterface)
            , name{ name }
            , parent{ parent }
        {
        }
        ~InterfaceDecl();

    public:
        std::string name;
        std::vector<FuncDecl*> children;
        ASTDecl* parent = nullptr;
    };


    class EnumDecl : public ASTDecl
    {
    public:
        EnumDecl(const std::string_view& name, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kEnum)
            , name{ name }
            , parent{ parent }
        {
        }
        ~EnumDecl();

    public:
        std::string name;
        std::vector<VarDecl*> children;
        ASTDecl* parent = nullptr;
    };


    class ModuleDecl : public ASTDecl
    {
    public:
        ModuleDecl(const std::string_view& name, ModuleDecl* parent = nullptr)
            : ASTDecl(DeclKind::kModule)
            , parent{ parent }
            , name{ name }
        {
        }
        ~ModuleDecl();

    public:
        class TopLevelDecls* children;
        std::string name;
        ModuleDecl* parent = nullptr;
    };


    class TopLevelDecls : public ASTDecl 
    {
    public:
        TopLevelDecls()
            : ASTDecl(DeclKind::kTopLevelDecls)
        {}
        ~TopLevelDecls();
    
    public:
        std::vector<ASTDecl*> decls;
    };
}