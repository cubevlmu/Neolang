#pragma once

#include "ast/ASTBase.hpp"
#include "ast/ASTType.hpp"

namespace neo::ast {

    class VarDecl : public ASTDecl
    {
    public:
        VarDecl(const std::string_view& name, ASTTypeRef type, ASTExpr* init = nullptr, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kVar)
            , name {name}
            , parent {parent} 
            , type {type}
            , initExpr {init}
        {}
        ~VarDecl() = default;

    public:
        ASTDecl* parent = nullptr;
        std::string name;
        ASTTypeRef type;
        ASTExpr* initExpr = nullptr;
        bool isConst = false;
        bool isStatic = false;
    };


    class FuncDecl : public ASTDecl 
    {
    public:
        FuncDecl(const std::string_view& name, ASTTypeRef retType, std::vector<VarDecl*> args, class CompoundStmt* body = nullptr, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kFunc)
            , name {name}
            , returnType {retType}
            , args {args}
            , funcBody {body}
            , parent {parent}
        {}
        ~FuncDecl() = default;
    
    public:
        std::string name;
        ASTTypeRef returnType;
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
        FieldDecl(const std::string_view& name, ASTTypeRef type, ASTDecl* parent, ASTExpr* init = nullptr)
            : ASTDecl(DeclKind::kField)
            , name {name}
            , parent {parent}
            , type {type}
            , init {init}
        {}
        ~FieldDecl() = default;

    public:
        std::string name;
        ASTDecl* parent;
        ASTTypeRef type;
        ASTExpr* init = nullptr;
    };


    class ClassDecl : public ASTDecl
    {
    public:
        ClassDecl(const std::string_view& name, std::vector<ASTTypeRef> baseClasses)
            : ASTDecl(DeclKind::kClass)
            , className {name}
            , baseClasses {baseClasses}
        {}
        ~ClassDecl() = default;

    public:
        std::string className;
        std::vector<ASTTypeRef> baseClasses;
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
            , name {name}
            , parent {parent}
        {}
        ~StructDecl() = default;

    public:
        std::string name;
        std::vector<VarDecl*> variables;
        std::vector<FieldDecl*> fields;
        ASTDecl* parent = nullptr;
    };


    class ModuleDecl : public ASTDecl
    {
    public:
        ModuleDecl(const std::string_view& name, ASTDecl* parent = nullptr)
            : ASTDecl(DeclKind::kModule)
            , parent {parent}
        {}
        ~ModuleDecl() = default;

    public:
        std::vector<ASTDecl*> childrens;
        ASTDecl* parent = nullptr;
    };
}