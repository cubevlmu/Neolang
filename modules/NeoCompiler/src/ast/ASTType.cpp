#include "ASTType.hpp"

#include "ast/ASTBase.hpp"
#include "Decl.hpp"

namespace neo::ast {


    static void buildTag(std::string& out, ModuleDecl* m) {
        if (m == nullptr) {
            out += "global.";
        }
        else {
            ModuleDecl* c = m;
            do {
                out.insert(0, c->name + ".");
                c = c->parent;
            } while (c != nullptr);
        }
    }

    class BuiltIns
    {
    public:
        static ASTTypeDef s_builtIn_i8;
        static ASTTypeDef s_builtIn_i16;
        static ASTTypeDef s_builtIn_i32;
        static ASTTypeDef s_builtIn_i64;
        static ASTTypeDef s_builtIn_u8;
        static ASTTypeDef s_builtIn_u16;
        static ASTTypeDef s_builtIn_u32;
        static ASTTypeDef s_builtIn_u64;
        static ASTTypeDef s_builtIn_f32;
        static ASTTypeDef s_builtIn_f64;
        static ASTTypeDef s_builtIn_bool;
        static ASTTypeDef s_builtIn_string;

        static void attach(TypeCache::TypeMap& map) {
            map.insert({ "i8", &s_builtIn_i8 });
            map.insert({ "char", &s_builtIn_i8 });
            map.insert({ "i16", &s_builtIn_i16 });
            map.insert({ "short", &s_builtIn_i16 });
            map.insert({ "i32", &s_builtIn_i32 });
            map.insert({ "int", &s_builtIn_i32 });
            map.insert({ "i64", &s_builtIn_i64 });
            map.insert({ "long", &s_builtIn_i64 });
            map.insert({ "u8", &s_builtIn_u8 });
            map.insert({ "byte", &s_builtIn_u8 });
            map.insert({ "u16", &s_builtIn_u16 });
            map.insert({ "ushort", &s_builtIn_u16 });
            map.insert({ "u32", &s_builtIn_u32 });
            map.insert({ "uint", &s_builtIn_u32 });
            map.insert({ "u64", &s_builtIn_u64 });
            map.insert({ "ulong", &s_builtIn_u64 });
            map.insert({ "f32", &s_builtIn_f32 });
            map.insert({ "float", &s_builtIn_f32 });
            map.insert({ "f64", &s_builtIn_f64 });
            map.insert({ "double", &s_builtIn_f64 });
            map.insert({ "bool", &s_builtIn_bool });
            map.insert({ "string", &s_builtIn_string });
        }
    };


    ASTTypeDef BuiltIns::s_builtIn_i8("i8");
    ASTTypeDef BuiltIns::s_builtIn_i16("i16");
    ASTTypeDef BuiltIns::s_builtIn_i32("i32");
    ASTTypeDef BuiltIns::s_builtIn_i64("i64");
    ASTTypeDef BuiltIns::s_builtIn_u8("u8");
    ASTTypeDef BuiltIns::s_builtIn_u16("u16");
    ASTTypeDef BuiltIns::s_builtIn_u32("u32");
    ASTTypeDef BuiltIns::s_builtIn_u64("u64");
    ASTTypeDef BuiltIns::s_builtIn_f32("f32");
    ASTTypeDef BuiltIns::s_builtIn_f64("f64");
    ASTTypeDef BuiltIns::s_builtIn_bool("bool");
    ASTTypeDef BuiltIns::s_builtIn_string("std.types.string");


    TypeCache::TypeCache()
    {
        BuiltIns::attach(m_defs);
    }


    TypeCache::~TypeCache()
    {
        m_defs.clear();
    }


    ASTTypeNode* TypeCache::getType(const std::string_view& raw_tag)
    {
        if (m_defs.find(raw_tag.data()) == m_defs.end()) {
            return nullptr; //TODO
        }
        auto* td = m_defs.at(raw_tag.data());
        return new ASTTypeNode {td};
    }


    ASTTypeNode* TypeCache::getType(ModuleDecl* m, const std::string_view& type_tag)
    {
        std::string idTag{};
        buildTag(idTag, m);
        idTag += type_tag;
        return getType(idTag.c_str());
    }


    ASTTypeNode* TypeCache::getType(const std::string_view& module_tag, const std::string_view& type_tag)
    {
        FastConcat(fullTag, module_tag.data(), type_tag.data());
        return getType(fullTag);
    }


    bool TypeCache::newType(ModuleDecl* m, StructDecl* s)
    {
        std::string idTag = "";
        buildTag(idTag, m);
        idTag += s->name;

        if (m_defs.contains(idTag)) {
            return false;
        }
        m_defs.insert({ idTag, new ASTTypeDef(m, s) });
        return m_defs.contains(idTag);
    }


    bool TypeCache::newType(ModuleDecl* m, ClassDecl* s)
    {
        std::string idTag = "";
        buildTag(idTag, m);
        idTag += s->name;

        if (m_defs.contains(idTag)) {
            return false;
        }
        m_defs.insert({ idTag, new ASTTypeDef(m, s) });
        return m_defs.contains(idTag);
    }


    bool TypeCache::newType(ModuleDecl* m, InterfaceDecl* s)
    {
        std::string idTag = "";
        buildTag(idTag, m);
        idTag += s->name;

        if (m_defs.contains(idTag)) {
            return false;
        }
        m_defs.insert({ idTag, new ASTTypeDef(m, s) });
        return m_defs.contains(idTag);
    }


    bool TypeCache::newType(ModuleDecl* m, EnumDecl* s)
    {
        std::string idTag = "";
        buildTag(idTag, m);
        idTag += s->name;

        if (m_defs.contains(idTag)) {
            return false;
        }
        m_defs.insert({ idTag, new ASTTypeDef(m, s) });
        return m_defs.contains(idTag);
    }


    ASTTypeNode::ASTTypeNode(ASTTypeDef* def)
        : ASTNode(ASTType::kTypeRef) 
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
    

    ASTTypeDef::ASTTypeDef(ModuleDecl* module, StructDecl* st)
        : ASTNode(ASTType::kTypeDef)
        , m_module{ module }
        , m_decl{ st }
    {
        std::string idTag = "";
        buildTag(idTag, module);
        idTag += st->name;

        m_rawId = idTag;
    }


    ASTTypeDef::ASTTypeDef(ModuleDecl* module, ClassDecl* st)
        : ASTNode(ASTType::kTypeDef)
        , m_module{ module }
        , m_decl{ st }
    {
        std::string idTag = "";
        buildTag(idTag, module);
        idTag += st->name;

        m_rawId = idTag;
    }


    ASTTypeDef::ASTTypeDef(ModuleDecl* module, InterfaceDecl* st)
        : ASTNode(ASTType::kTypeDef)
        , m_module{ module }
        , m_decl{ st }
    {
        std::string idTag = "";
        buildTag(idTag, module);
        idTag += st->name;

        m_rawId = idTag;
    }


    ASTTypeDef::ASTTypeDef(ModuleDecl* module, EnumDecl* st)
        : ASTNode(ASTType::kTypeDef)
        , m_module{ module }
        , m_decl{ st }
    {
        std::string idTag = "";
        buildTag(idTag, module);
        idTag += st->name;

        m_rawId = idTag;
    }


    ASTTypeDef::ASTTypeDef(const std::string_view& raw_id)
        : ASTNode(ASTType::kTypeDef)
        , m_module{ nullptr }
        , m_decl{ nullptr }
        , m_rawId{ raw_id }
        , m_isBuiltIn{ true }
    {
    }


    ASTTypeDef::~ASTTypeDef()
    {
    }
}