#include "Base.hpp"

#include "neo/compiler/DebugOutput.hpp"
#include "Base.hpp"
#include "neo/base/Serializer.hpp"

namespace neo {

    static const char* s_ASTTypeStrings[] = {  
        "kUnknown",
        "kStatment",
        "kDeclaration",
        "kType",
        "kTypeArray",
        "kTypePointer",
        "_________"
    };
    std::string_view getTypeString(ASTType type) {
        return s_ASTTypeStrings[(int)type];
    }


    static const char* s_StmtKindStrings[] = {
        "kUnknown",
        "kExpression",
        "kCompound",
        "kIf",
        "kWhile",
        "kFor",
        "kForeach",
        "kReturn",
        "kBreak",
        "kContinue",
        "kImport"
    };
    std::string_view getTypeString(StmtKind type) {
        return s_StmtKindStrings[(int)type];
    }


    static const char* s_ExprKindStrings[] = {
        "kUnknown",
        "kNumberLit",
        "kBoolLit",
        "kBinary",
        "kUnary",
        "kFuncCall",
        "kMemberAccess",
        "kVar",
        "kCast",
        "kNew"
    };
    std::string_view getTypeString(ExprKind type) {
        return s_ExprKindStrings[(int)type];
    }
    
    
    static const char* s_DeclKindStrings[] = {
        "kUnknown",
        "kVar",
        "kFunc",
        "kClass",
        "kField",
        "kStruct",
        "kModule",
        "kInterface",
        "kEnum",
        "kTopLevelDecls"
    };
    std::string_view getTypeString(DeclKind type) {
        return s_DeclKindStrings[(int)type];
    }



    void ASTNode::debugPrint(NDebugOutput& output) {
        return output.writeLine(toString().data());
    }
    

    void ASTNode::write(NSerializer* s)
    {
        // Write ASTType
        s->write((i32)m_type);
    }


    void ASTNode::read(NSerializer* s)
    {
        // Read ASTType
        i32 type = 0;
        s->read(type);
        m_type = (ASTType)type;
    }


    ASTDecl::~ASTDecl() {
        for (auto* ptr : attributes) {
            delete ptr;
        }
        attributes.clear();
    }


    ASTModifier::ASTModifier() noexcept
            : isStatic(false), isFinal(false), isConst(false),
              isPrivate(false), isProtected(false), isInternal(false), isInline(false) {}

    ASTModifier::ASTModifier(bool s, bool f, bool c, bool priv, bool prot, bool inter, bool inl) noexcept
            : isStatic(s), isFinal(f), isConst(c),
              isPrivate(priv), isProtected(prot), isInternal(inter), isInline(inl) {}

    ASTModifier::ASTModifier(const ASTModifier& other) noexcept
            : isStatic(other.isStatic), isFinal(other.isFinal), isConst(other.isConst),
              isPrivate(other.isPrivate), isProtected(other.isProtected),
              isInternal(other.isInternal), isInline(other.isInline) {}

    ASTModifier& ASTModifier::operator=(const ASTModifier& other) noexcept {
        if (this != &other) {
            isStatic = other.isStatic;
            isFinal = other.isFinal;
            isConst = other.isConst;
            isPrivate = other.isPrivate;
            isProtected = other.isProtected;
            isInternal = other.isInternal;
            isInline = other.isInline;
        }
        return *this;
    }

    ASTModifier::ASTModifier(ASTModifier&& other) noexcept
            : isStatic(other.isStatic), isFinal(other.isFinal), isConst(other.isConst),
              isPrivate(other.isPrivate), isProtected(other.isProtected),
              isInternal(other.isInternal), isInline(other.isInline) {
        // 对 bool 来说不需要清空 other
    }

    ASTModifier& ASTModifier::operator=(ASTModifier&& other) noexcept {
        if (this != &other) {
            isStatic = other.isStatic;
            isFinal = other.isFinal;
            isConst = other.isConst;
            isPrivate = other.isPrivate;
            isProtected = other.isProtected;
            isInternal = other.isInternal;
            isInline = other.isInline;
        }
        return *this;
    }

    bool ASTModifier::operator==(const ASTModifier& other) const noexcept {
        return isStatic == other.isStatic &&
               isFinal == other.isFinal &&
               isConst == other.isConst &&
               isPrivate == other.isPrivate &&
               isProtected == other.isProtected &&
               isInternal == other.isInternal &&
               isInline == other.isInline;
    }

    bool ASTModifier::operator!=(const ASTModifier& other) const noexcept {
        return !(*this == other);
    }
}