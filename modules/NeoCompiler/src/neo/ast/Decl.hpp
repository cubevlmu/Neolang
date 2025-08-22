#pragma once

#include <utility>

#include "Base.hpp"
#include "Type.hpp"

namespace neo {

    class VarDecl : public ASTDecl
    {
    public:
        VarDecl() : ASTDecl(DeclKind::kVar) {}
        VarDecl(const std::string_view& name, ASTTypeNode* type, ASTExpr* init = nullptr)
            : ASTDecl(DeclKind::kVar)
            , name{ name }
            , type{ type }
            , initExpr{ init }
        {
        }

    public:
        void debugPrint(NDebugOutput& output) override;

        void read(NSerializer* s) override;
        void write(NSerializer* s) override;

    public:
        std::string name;
        ASTTypeNode* type = nullptr;
        ASTExpr* initExpr = nullptr;
    };


    class FuncDecl : public ASTDecl
    {
    public:
        FuncDecl() : ASTDecl(DeclKind::kFunc) {}
        FuncDecl(const std::string_view& name, ASTTypeNode* retType, std::vector<VarDecl*> args, class CompoundStmt* body = nullptr)
            : ASTDecl(DeclKind::kFunc)
            , name{ name }
            , args{std::move( args )}
            , returnType{ retType }
            , funcBody{ body }
        {
        }
        ~FuncDecl() override = default;

    public:
        void debugPrint(NDebugOutput& output) override;

    public:
        std::string name;
        std::vector<VarDecl*> args;
        ASTTypeNode* returnType = nullptr;
        CompoundStmt* funcBody = nullptr;
    };


    class FieldDecl : public ASTDecl
    {
    public:
        FieldDecl()
            : ASTDecl(DeclKind::kField) {}
        FieldDecl(const std::string_view& name, ASTTypeNode* type, ASTExpr* init = nullptr)
            : ASTDecl(DeclKind::kField)
            , name{ name }
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
        ASTTypeNode* type = nullptr;
        ASTExpr* init = nullptr;
    };


    class ClassDecl : public ASTDecl
    {
    public:
        ClassDecl() : ASTDecl(DeclKind::kClass) {}
        ClassDecl(const std::string_view& name, std::vector<ASTTypeNode*> baseClasses)
            : ASTDecl(DeclKind::kClass)
            , name{ name }
            , baseClasses{std::move( baseClasses )}
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
        FuncDecl* dtors = nullptr;
    };


    class StructDecl : public ASTDecl
    {
    public:
        StructDecl() : ASTDecl(DeclKind::kStruct) {}
        StructDecl(const std::string_view& name)
            : ASTDecl(DeclKind::kStruct)
            , name{ name }
        {
        }
        ~StructDecl() override = default;

    public:
        std::string name;
        std::vector<VarDecl*> variables;
        std::vector<FieldDecl*> fields;
    };


    class InterfaceDecl : public ASTDecl
    {
    public:
        InterfaceDecl() : ASTDecl(DeclKind::kInterface) {}
        InterfaceDecl(const std::string_view& name)
            : ASTDecl(DeclKind::kInterface)
            , name{ name }
        {
        }
        ~InterfaceDecl() override;

    public:
        std::string name;
        std::vector<FuncDecl*> children;
    };


    class EnumDecl : public ASTDecl
    {
    public:
        EnumDecl() : ASTDecl(DeclKind::kEnum) {}
        EnumDecl(const std::string_view& name)
            : ASTDecl(DeclKind::kEnum)
            , name{ name }
            , baseType{nullptr}
        {
        }
        ~EnumDecl() override;

    public:
        std::string name;
        std::vector<VarDecl*> children;
        ASTTypeNode* baseType = nullptr;
    };


    class ModuleDecl : public ASTDecl
    {
    public:
        ModuleDecl() : ASTDecl(DeclKind::kModule) {}
        ModuleDecl(const std::string_view& name)
            : ASTDecl(DeclKind::kModule)
            , name{ name }
        {
        }
        ~ModuleDecl() override;

    public:
        std::string name;
        class TopLevelDecls* children = nullptr;
    };


    class TopLevelDecls : public ASTDecl 
    {
    public:
        TopLevelDecls() : ASTDecl(DeclKind::kTopLevelDecls) {}
        ~TopLevelDecls() override;
    
    public:
        std::vector<ASTDecl*> decls;
    };
}