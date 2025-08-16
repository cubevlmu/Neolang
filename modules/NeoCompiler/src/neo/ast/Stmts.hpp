#pragma once

#include "Base.hpp"
#include <initializer_list>
#include <utility>
#include <vector>

namespace neo {

    class CompoundStmt : public ASTStmt
    {
    public:
        CompoundStmt(std::vector<ASTStmt*> stmts)
            : ASTStmt(StmtKind::kCompound)
            , statements{std::move( stmts )}
        {
        }
        CompoundStmt(std::initializer_list<ASTStmt*> stmts)
            : ASTStmt(StmtKind::kCompound)
            , statements{ stmts }
        {
        }
        ~CompoundStmt() override = default;

    public:
        std::string toString() const override {
            std::string str{};
            for (auto& stmt : statements) {
                str += stmt->toString();
                str += '\n';
            }
            return str;
        };

    public:
        std::vector<ASTStmt*> statements;
    };


    class IfStmt final : public ASTStmt
    {
    public:
        IfStmt(ASTExpr* ifExpr, ASTStmt* defaultBranch, ASTStmt* elseBranch = nullptr)
            : ASTStmt(StmtKind::kIf)
            , ifExpr{ ifExpr }
            , defaultBranch{ defaultBranch }
            , elseBranch{ elseBranch }
        {
        }
        ~IfStmt() override = default;

    public:
        ASTExpr* ifExpr;
        ASTStmt* defaultBranch;
        ASTStmt* elseBranch = nullptr;;
    };


    class WhileStmt : public ASTStmt
    {
    public:
        WhileStmt(ASTExpr* cond, ASTStmt* body)
            : ASTStmt(StmtKind::kWhile)
            , condition{ cond }
            , body{ body }
        {
        }
        ~WhileStmt() override = default;

    public:
        ASTExpr* condition;
        ASTStmt* body;
    };


    class ForStmt : public ASTStmt
    {
    public:
        ForStmt(ASTStmt* decl, ASTExpr* cond, ASTExpr* update, ASTStmt* body)
            : ASTStmt(StmtKind::kFor)
            , declVar{ decl }
            , cond{ cond }
            , update{ update }
            , forBody{ body }
        {
        }
        ~ForStmt() override = default;

    public:
        ASTStmt* declVar;
        ASTExpr* cond;
        ASTExpr* update;
        ASTStmt* forBody;
    };


    class ForeachStmt : public ASTStmt
    {
    public:
        ForeachStmt(ASTStmt* decl, ASTExpr* object)
            : ASTStmt(StmtKind::kForeach)
            , declearation{ decl }
            , object{ object }
        {
        }
        ~ForeachStmt() override = default;

    public:
        ASTStmt* declearation;
        ASTExpr* object;
    };


    class ReturnStmt : public ASTStmt
    {
    public:
        ReturnStmt(ASTExpr* expr)
            : ASTStmt(StmtKind::kReturn)
            , ret{ expr }
        {
        }
        ~ReturnStmt() override = default;

    public:
        ASTExpr* ret;
    };


    class BreakStmt : public ASTStmt
    {
    public:
        BreakStmt() : ASTStmt(StmtKind::kBreak) {}
        ~BreakStmt() override = default;
    };


    class ContinueStmt : public ASTStmt
    {
    public:
        ContinueStmt() : ASTStmt(StmtKind::kContinue) {}
        ~ContinueStmt() override = default;
    };


    class ImportStmt : public ASTStmt 
    {
    public:
        ImportStmt(std::string name)
            : ASTStmt(StmtKind::kImport)
            , moduleName {std::move(name)}
        {}
        ~ImportStmt() override = default;

    public:
        const std::string moduleName;
    };

}