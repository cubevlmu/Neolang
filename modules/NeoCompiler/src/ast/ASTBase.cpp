#include "ASTBase.hpp"

#include "base/NDebugOutput.hpp"
#include "ASTBase.hpp"
#include "base/Serializer.hpp"

namespace neo::ast {

    static const char* s_ASTTypeStrings[] = {  
        "kUnknown",
        "kStatment",
        "kDeclaration",
        "kArrayType",
        "kPointer",
        "kTypeRef"
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
    

    void ASTNode::serialize(SGSerializer& s)
    {
        // Write ASTType
        s.write((i32)m_type);
    }


    void ASTNode::deserialize(SGSerializer& s)
    {
        // Read ASTType
        i32 type = 0;
        s.read(type);
        m_type = (ASTType)type;

    }


    ASTDecl::~ASTDecl() {
        for (auto* ptr : attributes) {
            delete ptr;
        }
        attributes.clear();
    }
}