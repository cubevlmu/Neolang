#pragma once

#include "ASTBase.hpp"
#include "ASTType.hpp"
#include <initializer_list>

namespace neo::ast {

    enum class LiteralType {
        kUnknown,
        kU8, kU16, kU32, kU64,
        kI8, kI16, kI32, kI64,
        kF32, kF64,
        kBool
    };
    std::string_view getTypeString(LiteralType);


    class NumberLiteralExpr : public ASTExpr
    {
    public:
        template <typename T>
        NumberLiteralExpr(T num)
            : ASTExpr(ExprKind::kNumberLit)
        {
            setNumber(num);
        }
        ~NumberLiteralExpr() = default;

    public:
        template <typename T>
        T getNumber() {
            switch (m_type)
            {
            case LiteralType::kU8:
                if constexpr (std::is_same_v<T, u8>)
                    return m_value.u8;
                break;
            case LiteralType::kU16:
                if constexpr (std::is_same_v<T, u16>)
                    return m_value.u16;
                break;
            case LiteralType::kU32:
                if constexpr (std::is_same_v<T, u32>)
                    return m_value.u32;
                break;
            case LiteralType::kU64:
                if constexpr (std::is_same_v<T, u64>)
                    return m_value.u64;
                break;
            case LiteralType::kI8:
                if constexpr (std::is_same_v<T, i8>)
                    return m_value.i8;
                break;
            case LiteralType::kI16:
                if constexpr (std::is_same_v<T, i16>)
                    return m_value.i16;
                break;
            case LiteralType::kI32:
                if constexpr (std::is_same_v<T, i32>)
                    return m_value.i32;
                break;
            case LiteralType::kI64:
                if constexpr (std::is_same_v<T, i64>)
                    return m_value.i64;
                break;
            case LiteralType::kF32:
                if constexpr (std::is_same_v<T, f32>)
                    return m_value.f32;
                break;
            case LiteralType::kF64:
                if constexpr (std::is_same_v<T, f64>)
                    return m_value.f64;
                break;
            default:
                FLIB_ASSERT(false && "type no support");
            }
        }
        std::string getNumberString() const;

        template <typename T>
        void setNumber(T num) {
            if constexpr (std::is_same_v<T, i8>) {
                m_value.i8 = num;
                m_type = LiteralType::kI8;
            }
            else if constexpr (std::is_same_v<T, i16>) {
                m_value.i16 = num;
                m_type = LiteralType::kI16;
            }
            else if constexpr (std::is_same_v<T, i32>) {
                m_value.i32 = num;
                m_type = LiteralType::kI32;
            }
            else if constexpr (std::is_same_v<T, i64>) {
                m_value.i64 = num;
                m_type = LiteralType::kI64;
            }
            else if constexpr (std::is_same_v<T, u8>) {
                m_value.u8 = num;
                m_type = LiteralType::kU8;
            }
            else if constexpr (std::is_same_v<T, u16>) {
                m_value.u16 = num;
                m_type = LiteralType::kU16;
            }
            else if constexpr (std::is_same_v<T, u32>) {
                m_value.u32 = num;
                m_type = LiteralType::kU32;
            }
            else if constexpr (std::is_same_v<T, u64>) {
                m_value.u64 = num;
                m_type = LiteralType::kU64;
            }
            else if constexpr (std::is_same_v<T, f32>) {
                m_value.f32 = num;
                m_type = LiteralType::kF32;
            }
            else if constexpr (std::is_same_v<T, f64>) {
                m_value.f64 = num;
                m_type = LiteralType::kF64;
            }
            else {
                FLIB_ASSERT(false && "not support");
            }
        }

        FLIB_FORCE_INLINE LiteralType getType() const {
            return m_type;
        }

        virtual std::string toString() const override;

    public:
        union {
            u8 u8; u16 u16; u32 u32; u64 u64;
            i8 i8; i16 i16; i32 i32; i64 i64;
            f32 f32; f64 f64;
        } m_value;
        LiteralType m_type = LiteralType::kUnknown;
    };


    class BoolLiteralExpr : public ASTExpr
    {
    public:
        BoolLiteralExpr(bool val)
            : ASTExpr(ExprKind::kBoolLit)
            , m_val{ val }
        {
        }
        ~BoolLiteralExpr() = default;

    public:
        FLIB_FORCE_INLINE LiteralType getType() const {
            return LiteralType::kBool;
        }
        virtual std::string toString() const override;

        FLIB_FORCE_INLINE void setValue(bool v) {
            m_val = v;
        }
        FLIB_FORCE_INLINE bool getValue() const {
            return m_val;
        }

    private:
        bool m_val;
    };


    enum class BinaryOp {
        kUnknown,
        kAdd, kSub, kMul, kDiv, kMod,
        kEq, kNeq, kLt, kLe, kGt, kGe,
        kBitAnd, kBitOr, kBitXor, kShl, kShr,
        kLAnd, kLOr
    };
    std::string_view getTypeString(BinaryOp);


    class BinaryExpr : public ASTExpr
    {
    public:
        BinaryExpr(BinaryOp type, ASTExpr* l, ASTExpr* r)
            : ASTExpr(ExprKind::kBinary)
            , op{ type }
            , left{ l }
            , right{ r }
        {
        }
        ~BinaryExpr() = default;

    public:
        virtual std::string toString() const override;

    public:
        ASTExpr* left;
        ASTExpr* right;
        const BinaryOp op;
    };


    enum class UnaryOp {
        kUnknown,
        kPlus,           // +a
        kMinus,          // -a
        kLogicalNot,     // !a
        kBitwiseNot,     // ~a
        kPreIncrement,   // ++a
        kPreDecrement,   // --a
        kPostIncrement,  // a++
        kPostDecrement   // a--
    };
    std::string_view toTypeString(UnaryOp);


    class UnaryExpr : public ASTExpr
    {
    public:
        UnaryExpr(UnaryOp op, ASTExpr* expr)
            : ASTExpr(ExprKind::kUnary)
            , op{ op }
            , operand{ expr }
        {
        }
        ~UnaryExpr() = default;

    public:
        virtual std::string toString() const override {
            return toTypeString(op).data() + operand->toString();
        }

    public:
        UnaryOp op;
        ASTExpr* operand;
    };


    class CallExpr : public ASTExpr
    {
    public:
        CallExpr(ASTExpr* fTag, std::vector<ASTExpr*> args)
            : ASTExpr(ExprKind::kFuncCall)
            , funcTag{ fTag }
            , callArgs{ args }
        {
        }
        CallExpr(ASTExpr* fTag, std::initializer_list<ASTExpr*> args)
            : ASTExpr(ExprKind::kFuncCall)
            , funcTag{ fTag }
            , callArgs{ args }
        {
        }
        ~CallExpr() = default;

    public:
        virtual std::string toString() const override {
            std::string str = funcTag->toString();
            for (auto& args : callArgs) {
                str.append(" ").append(args->toString());
            }
            return str;
        }

    public:
        ASTExpr* funcTag;
        std::vector<ASTExpr*> callArgs;
    };


    class MemberAccessExpr : public ASTExpr
    {
    public:
        MemberAccessExpr(ASTExpr* object, const std::string& member)
            : ASTExpr(ExprKind::kMemberAccess)
            , object{ object }
            , member{ member }
        {
        }
        ~MemberAccessExpr() = default;

    public:
        virtual std::string toString() const override {
            return object->toString() + (isUnsafeAccess ? "->" : ".") + member;
        }

    public:
        ASTExpr* object;
        std::string member;
        bool isUnsafeAccess = false;
    };


    class VariableRefExpr : public ASTExpr
    {
    public:
        VariableRefExpr(const std::string& name)
            : ASTExpr(ExprKind::kVar)
            , variableName{ name }
        {
        }
        ~VariableRefExpr() = default;

    public:
        virtual std::string toString() const override {
            return variableName;
        }

    public:
        std::string variableName;
        bool moveVariable = false;
    };


    class CastExpr : public ASTExpr
    {
    public:
        CastExpr(ASTExpr* obj, ASTTypeRef type)
            : ASTExpr(ExprKind::kCast)
            , castTo{ type }
            , object{ obj }
        {
        }
        ~CastExpr() = default;

    public:
        virtual std::string toString() const override {
            return std::string {"("} + castTo.getTag().data() + ")" + object->toString();
        }

    public:
        ASTTypeRef castTo;
        ASTExpr* object;
    };


    class NewExpr : public ASTExpr 
    {
    public:
        NewExpr(ASTTypeRef type, std::vector<ASTExpr*> args)
            : ASTExpr(ExprKind::kNew)
            , type {type}
            , arguments {args} 
        {}
        NewExpr(ASTTypeRef type, std::initializer_list<ASTExpr*> args) 
            : ASTExpr(ExprKind::kNew)
            , type {type}
            , arguments {args}
        {}
        ~NewExpr() = default;

    public:
        virtual std::string toString() const override {
            std::string str {"new "};
            str += type.getTag().data();
            str += "(";

            for (auto& arg : arguments) {
                str += arg->toString();
                str += " ";
            }

            str += ")";

            return str;
        }

    public:
        ASTTypeRef type;
        std::vector<ASTExpr*> arguments;
        bool isStackAlloc;
    };
}