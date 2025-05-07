#pragma once

#include "ASTBase.hpp"
#include <unordered_map>

namespace neo::ast {

    struct ASTTypeRef
    {
        friend class TypeCache;
    public:
        ASTTypeRef(const ASTTypeRef& b) {
            m_ptr = b.m_ptr;
            m_tag = b.m_tag;
        }
        ASTTypeRef(ASTTypeRef&& b) {
            m_ptr = b.m_ptr;
            m_tag = b.m_tag;
        }
        ASTTypeRef& operator=(ASTTypeRef&& b) {
            m_ptr = b.m_ptr;
            m_tag = b.m_tag;
            return *this;
        }
        ASTTypeRef() : m_ptr{ nullptr }, m_tag{ "" } {}
        ASTTypeRef(class ASTTypeDef*);
        ~ASTTypeRef() = default;

    public:
        class ASTTypeDef* getType();
        std::string_view getTag() const {
            return m_tag;
        }
        bool hasType() { return m_ptr; }


        ASTTypeRef& operator=(const ASTTypeRef& b)
        {
            m_tag = b.m_tag;
            m_ptr = b.m_ptr;
            return *this;
        }
        bool operator==(const ASTTypeRef& b) const
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
        bool operator!=(const ASTTypeRef& b) const
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

    public:
        static ASTTypeRef InvalidRef;

    private:
        std::string_view m_tag;
        ASTTypeDef* m_ptr;
    };


    class ASTTypeDef : ASTNode
    {
        friend class TypeCache;
        friend class ImportExpr;

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
    };


    class TypeCache
    {
    public:
        ASTTypeRef getType(const std::string_view& raw_tag);
        ASTTypeRef getType(ModuleDecl*, const std::string_view& type_tag);
        ASTTypeRef getType(const std::string_view& module_tag, const std::string_view& type_tag);

        bool newType(class ModuleDecl*, class StructDecl*);
        bool newType(class ModuleDecl*, class ClassDecl*);
        bool newType(class ModuleDecl*, class InterfaceDecl*);
        bool newType(class ModuleDecl*, class EnumDecl*);

    private:
        std::unordered_map<std::string, ASTTypeDef*> m_defs;
    };

}