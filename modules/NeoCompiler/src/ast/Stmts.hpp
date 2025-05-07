#pragma once

#include "ast/ASTBase.hpp"
#include <initializer_list>

namespace neo::ast {

    class CompoundStmt : public ASTStmt
    {
    public:
        CompoundStmt(std::vector<ASTStmt*> stmts)
            : ASTStmt(StmtKind::kCompound)
            , statements{ stmts }
        {
        }
        CompoundStmt(std::initializer_list<ASTStmt*> stmts)
            : ASTStmt(StmtKind::kCompound)
            , statements{ stmts }
        {
        }
        ~CompoundStmt() = default;

    public:
        virtual std::string toString() const {
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


    class IfStmt : public ASTStmt
    {
    public:
        IfStmt(ASTExpr* ifExpr, ASTStmt* defaultBranch, ASTStmt* elseBranch = nullptr)
            : ASTStmt(StmtKind::kIf)
            , ifExpr{ ifExpr }
            , defaultBranch{ defaultBranch }
            , elseBranch{ elseBranch }
        {
        }
        ~IfStmt() = default;

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
        ~WhileStmt() = default;

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
        ~ForStmt() = default;

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
        ~ForeachStmt() = default;

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
        ~ReturnStmt() = default;

    public:
        ASTExpr* ret;
    };


    class BreakStmt : public ASTStmt
    {
    public:
        BreakStmt() : ASTStmt(StmtKind::kBreak) {}
        ~BreakStmt() = default;
    };


    class ContinueStmt : public ASTStmt
    {
    public:
        ContinueStmt() : ASTStmt(StmtKind::kContinue) {}
        ~ContinueStmt() = default;
    };

}