#include "ASTType.hpp"

namespace neo::ast {


    ASTTypeRef TypeCache::getType(const std::string_view& raw_tag)
    {
        if (m_defs.find(raw_tag.data()) == m_defs.end()) {
            return ASTTypeRef::InvalidRef;
        }
        auto* td = m_defs.at(raw_tag.data());
        return td;
    }


    ASTTypeRef TypeCache::getType(ModuleDecl*, const std::string_view& type_tag)
    {
        return ASTTypeRef();
    }


    ASTTypeRef TypeCache::getType(const std::string_view& module_tag, const std::string_view& type_tag)
    {
        FastConcat(fullTag, module_tag.data(), type_tag.data());
        return getType(fullTag);
    }

    ASTTypeRef ASTTypeRef::InvalidRef {nullptr};

    ASTTypeRef::ASTTypeRef(ASTTypeDef* def)
    {
        if (def == nullptr)
            return;
        m_ptr = def;
        m_tag = def->getRawId();
    }


    std::string_view ASTTypeDef::getRawId()
    {
        return m_rawId;
    }
}