#include "Tokens.hpp"
#include <map>

namespace neo {

    static const char* s_typeStrings[] = {
        "kUnknown",
        "kEOF",

        // 标识符
        "kIdentifier",
        "kTypeId",

        // 模块系统
        "kImport",
        "kExport",
        "kInline",
        "kFinal",
        "kModule",
        "kExtern",

        // 函数与控制流
        "kFun",
        "kIf", "kElse",
        "kFor", "kWhile",

        // 类型定义
        "kClass",
        "kStruct",
        "kInterface",
        "kAttribute",
        "kEnum",

        // 构造器与成员
        "kCtor",
        "kDtor",
        "kField",

        // 可见性与修饰符
        "kPrivate", "kProtected", "kStatic",
        "kVirtual",
        "kInternal",
        "kOverride",
        "kImpl",

        // 变量声明
        "kVar",
        "kVal",
        "kConst",

        // 算术运算符
        "kAdd",
        "kSub",
        "kMul",
        "kDiv",
        "kMod",

        // 其他运算
        "kIsEqual",

        // 括号类
        "kLBracket", "kRBracket",
        "kLBraces", "kRBraces",
        "kLParen", "kRParen",

        // 字面量
        "kCharLit",
        "kStringLit",
        "kIntLit",
        "kHexLit",
        "kFloatLit",

        // 常量
        "kTrue", "kFalse", "kNull",

        // 特殊用法
        "kFuncCall",
        "kReturn",

        // 分隔符
        "kComma",
        "kDot",
        "kColon",
        "kSemicolon",
        "kQuestion",
        "kArrow",

        // 逻辑运算符
        "kLAnd",
        "kLOr",
        "kLNot",

        // 比较运算符
        "kEq",
        "kNeq",
        "kLt",
        "kGt",
        "kLe",
        "kGe",

        // 位运算符
        "kInc",
        "kDec",
        "kShl",
        "kShr",
        "kBitAnd",
        "kBitOr",
        "kBitXor",
        "kBitNot",

        // 赋值与复合赋值
        "kAssign",
        "kAddAssign",
        "kSubAssign",
        "kMulAssign",
        "kDivAssign",
        "kModAssign",
        "kShlAssign",
        "kShrAssign",
        "kAndAssign",
        "kOrAssign",
        "kXorAssign",

        // 控制流补充
        "kTry", "kCatch", "kFinally", "kThrow",
        "kBreak", "kContinue",
        "kNew"
    };
    static_assert(sizeof(s_typeStrings) / sizeof(s_typeStrings[0]) == static_cast<size_t>(NToken::kNew) + 1,
              "s_typeStrings array size does not match TokenType enum count");


    static const std::map<std::string_view, NToken::TokenType> s_idTypes{
        {"var", NToken::kVar},
        {"val", NToken::kVal},
        {"fun", NToken::kFun},
        {"class", NToken::kClass},
        {"struct", NToken::kStruct},
        {"import", NToken::kImport},
        {"module", NToken::kModule},
        {"export", NToken::kExport},
        {"const", NToken::kConst},
        {"ctor", NToken::kCtor},
        {"dtor", NToken::kDtor},
        {"private", NToken::kPrivate},
        {"protected", NToken::kProtected},
        {"internal", NToken::kInternal},
        {"impl", NToken::kImpl},
        {"if", NToken::kIf},
        {"else", NToken::kElse},
        {"inline", NToken::kInline},
        {"while", NToken::kWhile},
        {"for", NToken::kFor},
        {"interface", NToken::kInterface},
        {"virtual", NToken::kVirtual},
        {"enum", NToken::kEnum},
        {"false", NToken::kFalse},
        {"true", NToken::kTrue},
        {"field", NToken::kField},
        {"try", NToken::kTry},
        {"catch", NToken::kCatch},
        {"finally", NToken::kFinally},
        {"throw", NToken::kThrow},
        {"continue", NToken::kContinue},
        {"break", NToken::kBreak},
        {"new", NToken::kNew},
        {"extern", NToken::kExtern},
        {"final", NToken::kFinal}
    };


    NToken::TokenType NToken::checkIdentifier(const std::string_view& c)
    {
        std::string_view rc = c.data();
        if (s_idTypes.find(rc) != s_idTypes.end()) {
            return s_idTypes.at(rc);
        }
        return TokenType::kUnknown;
    }


    std::string_view NToken::typeString(TokenType tk)
    {
        int idx = (int)tk;
        if (idx < 0 || idx > 86) {
            return "";
        }
        return s_typeStrings[idx];
    }


    std::string NToken::toString() const
    {
        return "{ " + std::string(s_typeStrings[(int)type]) + " : " + value + " }";
    }


    std::string_view NToken::typeString() const
    {
        return typeString(type);
    }


    NToken NToken::InvalidToken {kUnknown, ""};
}