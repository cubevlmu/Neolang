#pragma once

#include <common.hpp>

#include "diagnose/SourceLoc.hpp"

namespace neo {

    struct NToken
    {
        enum TokenType {
            kUnknown,       // 未知符号
            kEOF,           // 文件结束

            // 标识符和类型名
            kIdentifier,    // 普通标识符
            kTypeId,        // 类型标识符（可选视语法是否需要区分）

            // 模块系统关键字
            kImport,        // import
            kExport,        // export
            kInline,        // inline（函数或模块 inline 化）
            kFinal,         // final
            kModule,        // module（用于模块化语言）
            kExtern,        // extern

            // 函数与控制流
            kFun,           // fun 或 function 关键字
            kIf, kElse,     // 条件语句
            kFor, kWhile,   // 循环控制语句

            // 类型定义与类相关关键字
            kClass,         // class
            kStruct,        // struct
            kInterface,     // interface
            kAttribute,     // 注解或属性定义
            kEnum,          // enum 枚举

            // 构造器与成员定义
            kCtor,          // 构造函数 ctor
            kDtor,          // 析构函数 dtor
            kField,         // 字段声明（如 field 或 property）

            // 可见性与修饰符
            kPrivate, kProtected, kStatic,
            kVirtual,       // virtual 函数
            kInternal,      // internal（模块私有）
            kOverride,      // override
            kImpl,          // implements / implement

            // 变量声明
            kVar,           // 可变变量
            kVal,           // 不可变变量（类似 Kotlin）
            kConst,         // 常量

            // 算术运算符
            kAdd,           // +
            kSub,           // -
            kMul,           // *
            kDiv,           // /
            kMod,           // %

            // 其他运算
            kIsEqual,       // ===（如果语言有严格等于，建议改名为 kStrictEq）

            // 括号类
            kLBracket, kRBracket, // [ ]
            kLBraces, kRBraces,   // { }
            kLParen, kRParen,     // ( )

            // 字面量
            kCharLit,     // 字符字面量，例如 'a'
            kStringLit,   // 字符串字面量
            kIntLit,      // 整数字面量
            kHexLit,      // 
            kFloatLit,    // 浮点数字面量

            // 常量关键字
            kTrue, kFalse, kNull, // 布尔和空值

            // 特殊用法
            kFuncCall,    // 可能是你为处理函数调用做的特殊 token（视语法可保留或删掉）
            kReturn,      // return

            // 分隔符
            kComma,       // ,
            kDot,         // .
            kColon,       // :
            kSemicolon,   // ;
            kQuestion,    // ?
            kArrow,       // ->

            // 逻辑运算符
            kLAnd,        // &&
            kLOr,         // ||
            kLNot,        // !

            // 比较运算符
            kEq,          // ==
            kNeq,         // !=
            kLt,          // <
            kGt,          // >
            kLe,          // <=
            kGe,          // >=

            // 位运算符
            kInc,         // ++
            kDec,         // --
            kShl,         // <<
            kShr,         // >>
            kBitAnd,      // &
            kBitOr,       // |
            kBitXor,      // ^
            kBitNot,      // ~

            // 赋值和复合赋值
            kAssign,      // =
            kAddAssign,   // +=
            kSubAssign,   // -=
            kMulAssign,   // *=
            kDivAssign,   // /=
            kModAssign,   // %=
            kShlAssign,   // <<=
            kShrAssign,   // >>=
            kAndAssign,   // &=
            kOrAssign,    // |=
            kXorAssign,   // ^=

            kTry, kCatch, kFinally, kThrow,
            kBreak, kContinue,
            kNew
        } type;
        std::string value;
        u32 line;
        u32 cursor;

        static std::string_view typeString(TokenType);
        static TokenType checkIdentifier(const std::string_view& str);
        std::string toString() const;
        std::string_view typeString() const;

        bool operator==(const NToken& other) const {
            return type == other.type && value == other.value;
        }
        bool operator!=(const NToken& other) const {
            return !(*this == other);
        }


        SourceLoc location(NSourceFile* file = nullptr) const {
            return SourceLoc { line, cursor, file };
        }

        static NToken InvalidToken;
    };
}