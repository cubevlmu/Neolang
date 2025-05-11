#pragma once

#include "ASTBase.hpp"
#include <unordered_map>

namespace neo::ast {

    struct ASTTypeNode : public ASTNode
    {
        friend class TypeCache;
    public:
        ASTTypeNode(const ASTTypeNode& b)
            : ASTNode(ASTType::kTypeRef) 
        {
            m_ptr = b.m_ptr;
            m_tag = b.m_tag;
        }
        ASTTypeNode(ASTTypeNode&& b)
            : ASTNode(ASTType::kTypeRef) 
        {
            m_ptr = b.m_ptr;
            m_tag = b.m_tag;
        }
        ASTTypeNode& operator=(ASTTypeNode&& b)
        {
            m_ptr = b.m_ptr;
            m_tag = b.m_tag;
            return *this;
        }
        ASTTypeNode() 
            : ASTNode(ASTType::kTypeRef) 
            , m_ptr{ nullptr }
            , m_tag{ "" } 
        {}
        ASTTypeNode(const std::string_view& id) 
            : ASTNode(ASTType::kTypeRef) 
            , m_ptr{nullptr}
            , m_tag{id} 
        {}
        ASTTypeNode(class ASTTypeDef*);
        ~ASTTypeNode() = default;

    public:
        class ASTTypeDef* getType();
        std::string_view getTag() const {
            return m_tag;
        }
        bool hasType() { return m_ptr; }


        ASTTypeNode& operator=(const ASTTypeNode& b)
        {
            m_tag = b.m_tag;
            m_ptr = b.m_ptr;
            return *this;
        }
        bool operator==(const ASTTypeNode& b) const
        {
            return m_ptr == b.m_ptr && m_tag == b.m_tag;
        }
        bool operator==(const std::string_view& code) const
        {
            return m_tag == m_tag;
        }
        bool operator==(const std::string& code) const
        {
            return m_tag == m_tag;
        }
        bool operator!=(const ASTTypeNode& b) const
        {
            return m_ptr != b.m_ptr || m_tag != b.m_tag;
        }
        bool operator!=(const std::string_view& code) const
        {
            return m_tag != m_tag;
        }
        bool operator!=(const std::string& code) const
        {
            return m_tag != m_tag;
        }
        explicit operator bool() const
        {
            return m_ptr != nullptr;
        }

    private:
        std::string_view m_tag;
        ASTTypeDef* m_ptr;
    };


    class ASTTypeDef : ASTNode
    {
        friend class TypeCache;
        friend class ImportExpr;
        friend class BuiltIns;

    private:
        ASTTypeDef(class ModuleDecl*, class StructDecl*);
        ASTTypeDef(class ModuleDecl*, class ClassDecl*);
        ASTTypeDef(class ModuleDecl*, class InterfaceDecl*);
        ASTTypeDef(class ModuleDecl*, class EnumDecl*);
        ASTTypeDef(const std::string_view& raw_id);
        ~ASTTypeDef();

    public:
        std::string_view getRawId();

    private:
        std::string m_rawId;
        ModuleDecl* m_module;
        ASTDecl* m_decl;

        bool m_isBuiltIn = false;
    };


    class TypeCache
    {
    public:
        using TypeMap = std::unordered_map<std::string, ASTTypeDef*>;

    public:
        TypeCache();
        ~TypeCache();

    public:
        ASTTypeNode* getType(const std::string_view& raw_tag);
        ASTTypeNode* getType(ModuleDecl*, const std::string_view& type_tag);
        ASTTypeNode* getType(const std::string_view& module_tag, const std::string_view& type_tag);

        bool newType(class ModuleDecl*, class StructDecl*);
        bool newType(class ModuleDecl*, class ClassDecl*);
        bool newType(class ModuleDecl*, class InterfaceDecl*);
        bool newType(class ModuleDecl*, class EnumDecl*);

    private:
        TypeMap m_defs;
    };


    class ASTArrayType : public ASTTypeNode
    {
    public:
        ASTArrayType(ASTTypeNode* type)
            : ASTNode(ASTType::kArrayType)
            , type{ type }
        {
        }
        ~ASTArrayType() = default;

    public:
        ASTTypeNode* type;
        bool isReceiver;
        int demission;
        std::vector<int> arraySize;
    };


    class ASTPointerType : public ASTNode
    {
    public:
        ASTPointerType(ASTTypeNode* type)
            : ASTNode(ASTType::kPointer)
            , type {type}
        {}
        ~ASTPointerType() = default;

    public:
        ASTTypeNode* type;
    };
}