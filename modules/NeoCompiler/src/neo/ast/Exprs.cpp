#include "Exprs.hpp"
#include "Base.hpp"

namespace neo {


    std::string_view getTypeString(LiteralType) {
        return ""; //TODO
    }
    std::string_view getTypeString(BinaryOp) {
        return ""; //TODO
    }
    std::string_view toTypeString(UnaryOp) {
        return ""; //TODO
    }


    std::string NumberLiteralExpr::getNumberString() const {
        switch (m_type) {
        case LiteralType::kU8: return std::to_string(m_value.u8);
        case LiteralType::kU16:return std::to_string(m_value.u16);
        case LiteralType::kU32:return std::to_string(m_value.u32);
        case LiteralType::kU64:return std::to_string(m_value.u64);
        case LiteralType::kI8: return std::to_string(m_value.i8);
        case LiteralType::kI16:return std::to_string(m_value.i16);
        case LiteralType::kI32:return std::to_string(m_value.i32);
        case LiteralType::kI64:return std::to_string(m_value.i64);
        case LiteralType::kF32:return std::to_string(m_value.f32);
        case LiteralType::kF64:return std::to_string(m_value.f64);
        case LiteralType::kUnknown:
        case LiteralType::kBool:
            return "";
        }
    }


    std::string NumberLiteralExpr::toString() const {
        return std::string {"["} + getTypeString(m_type).data() + "] " + getNumberString();
    }


    std::string BinaryExpr::toString() const {
        return left->toString() + " " + getTypeString(op).data() + " " + right->toString();
    }

    std::string BoolLiteralExpr::toString() const {
        return ASTNode::toString();
    }
}