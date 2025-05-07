#pragma once

#include <string>
#include <string_view>
#include <common.hpp>

namespace neo {
    class NDebugOutput;
    class SGSerializer;
}

namespace neo::ast {

    enum ASTType {
        kUnknown,
        kStatment,
        kDeclaration
    };
    std::string_view getTypeString(ASTType);


    class ASTNode
    {
    public:
        explicit ASTNode(ASTType type)
            : m_type{ type }
        {
        }
        virtual ~ASTNode() = default;

    public:
        virtual std::string toString() const {
            return { getTypeString(m_type).data() };
        };
        virtual void debugPrint(NDebugOutput& output);
        virtual void serialize(SGSerializer&);
        virtual void deserialize(SGSerializer&);

        ASTType getType() const {
            return m_type;
        }
        
        struct SourceLoc {
            u32 line;
            u32 column;
            class NSourceFile* file;
        } m_loc;

    private:
        ASTType m_type;
    };


    enum class StmtKind {
        kUnknown,
        kExpression,
        kCompound,
        kIf,
        kWhile,
        kFor,
        kForeach,
        kReturn,
        kBreak,
        kContinue
    };
    std::string_view getTypeString(StmtKind);


    class ASTStmt : public ASTNode
    {
    public:
        explicit ASTStmt(StmtKind kind)
            : ASTNode(ASTType::kStatment)
            , m_kind{ kind }
        {
        }
        ~ASTStmt() = default;

    public:
        FLIB_FORCE_INLINE StmtKind getStmtKind() const {
            return m_kind;
        }
        virtual void visit(class ASTVisitor& visitor) {}

    private:
        const StmtKind m_kind;
    };


    enum class ExprKind {
        kUnknown,
        kNumberLit,
        kBoolLit,
        kBinary,
        kUnary,
        kFuncCall,
        kMemberAccess,
        kVar,
        kCast,
        kNew
    };
    std::string_view getTypeString(ExprKind);


    class ASTExpr : public ASTStmt 
    {
    public:
        explicit ASTExpr(ExprKind kind)
            : ASTStmt(StmtKind::kExpression)
            , m_kind {kind} 
        {}
        ~ASTExpr() = default;

    public:
        FLIB_FORCE_INLINE ExprKind getExprKind() const { 
            return m_kind; 
        }

    private:
        ExprKind m_kind;
    };


    enum class DeclKind {
        kUnknown,
        kVar,
        kFunc,
        kClass,
        kField,
        kStruct,
        kModule
    };
    std::string_view getTypeString(DeclKind);

    
    class ASTDecl : public ASTNode 
    {
    public:
        explicit ASTDecl(DeclKind kind) 
            : ASTNode(ASTType::kDeclaration)
            , m_kind {kind} 
        {}
        ~ASTDecl() = default;

    public:
        FLIB_FORCE_INLINE DeclKind getDeclKind() const { 
            return m_kind; 
        }

    public:
        bool isMarkedExport;

    private:
        DeclKind m_kind;
    };
    
}