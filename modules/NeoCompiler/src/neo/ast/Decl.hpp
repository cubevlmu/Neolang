#pragma once

#include <utility>

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
        FuncDecl(const std::string_view& name, ASTTypeNode* retType, std::vector<VarDecl*> args, class CompoundStmt* body = nullptr)
            : ASTDecl(DeclKind::kFunc)
            , name{ name }
            , returnType{ retType }
            , args{std::move( args )}
            , funcBody{ body }
        {
        }
        ~FuncDecl() override = default;

    public:
        std::string name;
        ASTTypeNode* returnType;
        std::vector<VarDecl*> args;
        CompoundStmt* funcBody;

        bool isMarkedExtern;
        bool isMarkedInline;
        bool isClassFunc;
        bool isVirtualFunc;
        bool isOverrideFunc;
    };


    class FieldDecl : public ASTDecl
    {
    public:
        FieldDecl(const std::string_view& name, ASTTypeNode* type, ASTExpr* init = nullptr, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kField)
            , name{ name }
            , parent{ parent }
            , type{ type }
            , init{ init }
            , setFuncName{}
            , getFuncName{}
        {
        }
        ~FieldDecl() override = default;

    public:
        std::string name;
        std::string setFuncName;
        std::string getFuncName;
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
            , baseClasses{std::move( baseClasses )}
            , parent{ parent }
        {
        }
        ~ClassDecl() override = default;

    public:
        std::string name;
        std::vector<ASTTypeNode*> baseClasses;
        std::vector<ASTDecl*> subDataTypes;
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
        ~StructDecl() override = default;

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
        ~InterfaceDecl() override;

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
            , baseType{nullptr}
        {
        }
        ~EnumDecl() override;

    public:
        std::string name;
        std::vector<VarDecl*> children;
        ASTTypeNode* baseType;
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
        ~ModuleDecl() override;

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
        ~TopLevelDecls() override;
    
    public:
        std::vector<ASTDecl*> decls;
    };
}