#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "neo/common.hpp"
#include "neo/diagnose/SourceLoc.hpp"
#include "neo/base/Serializer.hpp"

namespace neo {

    class NDebugOutput;
    class NSerializer;

    enum ASTType {
        kUnknown,
        kStatment,
        kDeclaration,
        kType,
        kTypeArray,
        kTypePointer,
        kUnused1
    };
    std::string_view getTypeString(ASTType);


    struct Attribute {
        std::string name;
        std::vector<class ASTExpr*> arguments;
    };


    class ASTNode : public ISerializable
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

        virtual void read(NSerializer* s) override;
        virtual void write(NSerializer* s) override;

        ASTType getType() const {
            return m_type;
        }
        
        SourceLoc m_loc;

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
        kContinue,
        kImport
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
        ~ASTStmt() override = default;

    public:
        NE_FORCE_INLINE StmtKind getStmtKind() const {
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
        ~ASTExpr() override = default;

    public:
        NE_FORCE_INLINE ExprKind getExprKind() const {
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
        kModule,
        kInterface,
        kEnum,
        kTopLevelDecls
    };
    std::string_view getTypeString(DeclKind);

    
    class ASTDecl : public ASTNode 
    {
    public:
        explicit ASTDecl(DeclKind kind) 
            : ASTNode(ASTType::kDeclaration)
            , m_kind {kind} 
        {}
        ~ASTDecl() override;

    public:
        NE_FORCE_INLINE DeclKind getDeclKind() const {
            return m_kind; 
        }

    public:
        bool isMarkedExport;
        std::vector<Attribute*> attributes;

    private:
        DeclKind m_kind;
    };
    
}