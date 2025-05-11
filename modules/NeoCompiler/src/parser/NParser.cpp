#include "NParser.hpp"

#include "ast/ASTType.hpp"
#include "ast/Decl.hpp"
#include "base/StringUtils.hpp"
#include "diagnose/Diagnostic.hpp"
#include "lexer/NLexer.hpp"
#include "lexer/Tokens.hpp"
#include "parser/NParsedFile.hpp"
#include "ast/Stmts.hpp"

#include <sstream>
#include <vector>

namespace neo {

    template <typename T>
    struct ScopeGuard
    {
        ScopeGuard(T* ptr) : m_ptr{ ptr }, m_used{ false } {}
        ~ScopeGuard() {
            if (m_used) return;
            delete m_ptr;
        }

        T* getPtr() {
            m_used = true;
            return m_ptr;
        }

        T* operator->() {
            return m_ptr;
        }

    private:
        bool m_used;
        T* m_ptr;
    };


    struct NParser::Modifiers {
        bool isStatic;
        bool isFinal;
        bool isConst;
        bool isPrivate;
        bool isProtected;
        bool isInternal;
        bool isInline;

        void applyTo(ast::FuncDecl* f);
        void applyTo(ast::VarDecl* c);
        void applyTo(ast::FieldDecl* f);
    };


    NToken::TokenType NParser::s_modifier[] = {
        NToken::kInline,
        NToken::kStatic,
        NToken::kConst,
        NToken::kExport,
        NToken::kInternal,
        NToken::kProtected,
        NToken::kPrivate,
    };

#define ERRR() &m_diag, current(), m_args.file
#define CLEARUP(V) \
do { \
    for (auto* ptr : V) { \
        delete ptr; \
    } \
    V.clear();\
} while(false) \

    NParser::NParser(NParserArgs args)
        : m_args{ args }
        , m_lexer{ args.lexer }
    {
    }


    NParser::~NParser()
    {
        m_diag.clear();
    }


    NToken& NParser::advance() {
        return m_lexer->nextToken();
    }
    NToken& NParser::current() {
        return m_lexer->current();
    }
    NToken& NParser::peek() {
        return m_lexer->peekNext();
    }
    NToken& NParser::peekPrevious() {
        return m_lexer->peekPrevious();
    }
    NToken& NParser::previous() {
        return m_lexer->previousToken();
    }
    bool NParser::match(NToken::TokenType type)
    {
        return current().type == type;
    }
    bool NParser::expect(NToken::TokenType type)
    {
        return m_lexer->expectToken(type);
    }
    bool NParser::check(NToken::TokenType type)
    {
        return m_lexer->current().type == type;
    }


    Expected<void> NParser::parseRoot()
    {
        auto& output = m_args.output;

        while (!m_lexer->expectToken(NToken::kEOF)) {
            if (check(NToken::kImport)) {
                auto p_import_Ret = parseImport();
                CHECK_ERROR(p_import_Ret);
                output.Nodes.push_back(p_import_Ret.value());
            }
            else if (check(NToken::kModule)) {
                auto p_module_Ret = parseModule();
                CHECK_ERROR(p_module_Ret);
                output.Nodes.push_back(p_module_Ret.value());
            }
            // else if (check(NToken::kClass) || check(NToken::kStruct)) {
            //     if (!parseClass()) 
            //         return false;
            // }
            // else if (check(NToken::kInterface)) {

            // }
            // else if (check(NToken::kFun)) {
            //     if (!parseFunction()) return false;
            // }
            // else if (check(NToken::kVar) || check(NToken::kVal)) {
            //     if (!parseVariableDecl()) return false;
            // }
            else {
                return Error::failure("Unexpected token at top level.", ERRR());
            }
        }

        return Error::success();
    }


    Expected<ast::ASTTypeRef> NParser::parseType()
    {
        if (!check(NToken::kIdentifier)) {
            return ast::ASTTypeRef::InvalidRef;
        }
        std::string name = current().value;

        if (expect(NToken::kLBracket)) {
            advance();
            if (!expect(NToken::kRBracket)) {
                return Error::failure(msg("Array type need close bracket ']' but got '", peek().value, "'"), ERRR());
            }
            advance();
            return ast::ASTTypeRef {name};
        }

    }

    // tested
    Expected<ast::ImportStmt*> NParser::parseImport()
    {
        if (!check(NToken::kImport)) {
            return Error::failure(msg("unexpected token '", current().typeString(), "' for import expression"), &m_diag, current(), m_args.file);
        }
        std::stringstream moduleNameBuilder{};

        do {
            advance();
            if (check(NToken::kIdentifier)) {
                moduleNameBuilder << current().value;
            }
            else if (check(NToken::kDot)) {
                moduleNameBuilder << '.';
            }
            else {
                return Error::failure(msg("unexpected token '", current().typeString(), "' for module name"), ERRR());
            }
        } while (expect(NToken::kSemicolon));

        return new ast::ImportStmt(moduleNameBuilder.str());
    }


    Expected<ast::ModuleDecl*> NParser::parseModule()
    {
        if (!check(NToken::kModule)) {
            return Error::failure(msg("unexpected token '", current().typeString(), "' for import expression"), ERRR());
        }
        std::string module{};

        do {
            advance();
            if (check(NToken::kIdentifier)) {
                module = current().value;
            }
            else {
                return Error::failure(msg("unexpected token '", current().typeString(), "' for module declear"), ERRR());
            }
        } while (expect(NToken::kSemicolon) || expect(NToken::kLBraces));

        if (expect(NToken::kSemicolon)) {
            auto r = parseDecls();
            if (!r) {
                delete r.value();
                return r.error();
            }
            auto ptr = new ast::ModuleDecl(module, nullptr);
            ptr->children = r.value();
            return ptr;
        }
        else if (expect(NToken::kLBraces)) {
            auto r = parseScopeDecls();
            if (!r) {
                delete r.value();
                return r.error();
            }
            auto ptr = new ast::ModuleDecl(module, nullptr);
            ptr->children = r.value();
            return ptr;
        }
        else {
            return Error::failure(msg("expect ';' or '{' behind module declear statement, but found '", current().typeString(), "'"), ERRR());
        }
    }


    Expected<ast::TopLevelDecls*> NParser::parseDecls()
    {
        ScopeGuard dC{ new ast::TopLevelDecls {} };

        // logic
        if (check(NToken::kModule)) {
            auto p_module_Ret = parseModule();
            CHECK_ERROR(p_module_Ret);
            dC->decls.push_back(p_module_Ret.value());
        }
        else if (check(NToken::kClass)) {
            advance();
            auto p_class_Ret = parseClass();
            CHECK_ERROR(p_class_Ret);
            dC->decls.push_back(p_class_Ret.value());
        }
        //TODO other types
        else {
            return Error::failure(msg("unexpected token '", current().typeString(), "' found"), ERRR());
        }

        return dC.getPtr();
    }


    Expected<ast::TopLevelDecls*> NParser::parseScopeDecls()
    {
        if (!check(NToken::kLBraces)) {
            return Error::failure("trying parse declear scope but not in a scope", ERRR());
        }
        advance();

        auto r = parseDecls();
        if (!r) {
            return r.error();
        }

        if (!expect(NToken::kRBraces)) {
            return Error::failure(msg("scope not closed, expect '}' but found : '", current().typeString(), "'"), ERRR());
        }
        advance();

        return r.value();
    }


    Expected<ast::ClassDecl*> NParser::parseClass()
    {
        if (!check(NToken::kIdentifier)) {
            return Error::failure(msg("expected identifier for class name but got : '", current().typeString(), "'"), ERRR());
        }
        std::string& name = current().value;
        std::vector<ast::ASTTypeRef> baseClasses{};
        ScopeGuard g{ new ast::ClassDecl(name.c_str(), baseClasses) };


        if (check(NToken::kColon)) {
            if (!expect(NToken::kIdentifier)) {
                return Error::failure(msg("unexpect token '", current().typeString(), "' for base classes"), ERRR());
            }
            u32 commaCount = 0;
            do {
                advance();
                if (check(NToken::kIdentifier)) {
                    if (commaCount != baseClasses.size()) {
                        return Error::failure("comma count not match base class defines", ERRR());
                    }
                    g->baseClasses.push_back(ast::ASTTypeRef{ current().value });
                }
                else if (check(NToken::kComma)) {
                    commaCount++;
                }
                else {
                    return Error::failure(msg("unexpect token '", current().typeString(), "' for base classes"), ERRR());
                }
            } while (expect(NToken::kLBraces) || expect(NToken::kSemicolon));
            advance();

            if (check(NToken::kLBraces)) {
                goto parseBody;
            }
            else if (check(NToken::kSemicolon)) {
                goto end;
            }
            else {
                return Error::failure(msg("unexpect token '", current().typeString(), "' for class declear"), ERRR());
            }
        }
        else if (check(NToken::kLBraces)) {
            goto parseBody;
        }
        else if (check(NToken::kSemicolon)) {
            goto end;
        }
        else {
            return Error::failure(msg("unexpect token '", current().typeString(), "' for class declear"), ERRR());
        }

    parseBody:

        do {
            advance();

            // for class body parsing
            std::vector<ast::Attribute*> c_atts;
            Modifiers mf{};

            // todo
            if (check(NToken::kLBracket)) {
                auto r = parseAttributes();
                CHECK_ERROR(r);
                c_atts = r.value();
                advance();
            }

            if (std::find(&s_modifier[0], &s_modifier[7], current().type)) {
                auto r = parseModifier();
                CHECK_ERROR(r);
                mf = r.value();
            }

            if (check(NToken::kCtor)) {

            }
            else if (check(NToken::kDtor)) {

            }
            else if (check(NToken::kFun)) {

            }
            else if (check(NToken::kField)) {

            }
            else if (check(NToken::kVal) || check(NToken::kVar)) {

            }
            else if (check(NToken::kClass)) {
                auto r = parseClass();
                CHECK_ERROR(r);
                g->subClasses.push_back(r.value());
            }
            else if (check(NToken::kStruct)) {

            }
            else if (check(NToken::kInterface)) {

            }
            else {
                return Error::failure(msg("unexpected token '", current().value, "' for class member"), ERRR());
            }
        } while (expect(NToken::kRBraces));

    end:
        advance();
        return g.getPtr();
        // if (!check(NToken::kClass)) {
        //     return Error::failure("", ERRR());
        // }
        // advance();

        // ScopeGuard g {new ast::ClassDecl(const std::string_view &name, std::vector<ASTTypeRef> baseClasses)};
    }


    Expected<ast::FuncDecl*> NParser::parseFunc()
    {
        if (!check(NToken::kFun) && expect(NToken::kIdentifier)) {
            return Error::failure(msg("unexpected token for function declear : ", current().value, " ", peek().value), ERRR());
        }
        advance();
        std::string name = current().value;

        if (!expect(NToken::kLParen)) {
            return Error::failure(msg("function declear expect '(' for function arguments but got '", peek().value, "'"), ERRR());
        }
        advance();
        std::vector<ast::VarDecl*> args{};

        do {
            advance();
            if (check(NToken::kIdentifier) && expect(NToken::kColon)) {
                std::string& arg_name = current().value;
                advance();
                if (!expect(NToken::kIdentifier)) {
                    advance();
                    return Error::failure(msg("expect type identifier for function argument, but receive '", current().value, "'"), ERRR());
                }
                advance();
                ast::ASTTypeRef type{ current().value };
                args.push_back(new ast::VarDecl());
                //TODO
            }
        } while ();

    }


    Expected<std::vector<ast::Attribute*>> NParser::parseAttributes()
    {
        std::vector<ast::Attribute*> attrs{};

        do {
            if (!check(NToken::kLBracket) && expect(NToken::kIdentifier)) {
                return attrs;
            }
            advance();

            std::string name = current().value;
            ScopeGuard g{ new ast::Attribute {} };
            g->name = name;

            if (!expect(NToken::kLParen) && !expect(NToken::kRBracket)) {
                return attrs;
            }
            advance();
            if (check(NToken::kLParen)) {
                auto r = parseFuncCallArgs();
                CHECK_ERROR(r);
                g->arguments.swap(r.value());
                if (!expect(NToken::kRBracket)) {
                    CLEARUP(attrs);
                    return Error::failure(msg("expect ']' to close attribute attach but got '", current().value, "'"), ERRR());
                }
                attrs.push_back(g.getPtr());
            }
            else if (check(NToken::kRBracket)) {
                attrs.push_back(g.getPtr());
            }
            else {
                CLEARUP(attrs);
                return Error::failure(msg("unexpect token '", current().value, "' after attribute attach's name"), ERRR());
            }
        } while (expect(NToken::kLBracket));

        return attrs;
    }


    Expected<NParser::Modifiers> NParser::parseModifier()
    {
        Modifiers mf{};
        previous();

        do {
            advance();
            if (check(NToken::kPrivate)) {
                mf.isPrivate = true;
            }
            else if (check(NToken::kProtected)) {
                mf.isProtected = true;
            }
            else if (check(NToken::kInternal))
            {
                mf.isInternal = true;
            }
            else if (check(NToken::kInline))
            {
                mf.isInline = true;
            }
            else if (check(NToken::kStatic))
            {
                mf.isStatic = true;
            }
            else if (NToken::kConst)
            {
                mf.isConst = true;
            }
            else {
                return mf;
            }
        } while (std::find(&s_modifier[0], &s_modifier[7], peek().type));

        return mf;
    }


    Expected<std::vector<ast::ASTExpr*>> NParser::parseFuncCallArgs()
    {
        std::vector<ast::ASTExpr*> args{};
        if (!check(NToken::kLParen)) {
            return args;
        }

        do {
            advance();
            auto r = parseExpr();
            if (!r) {
                return r.error();
            }
            args.push_back(r.value());
        } while (expect(NToken::kRParen));

        return args;
    }


    bool NParser::parse()
    {
        auto& output = m_args.output;
        output.clearNodes();

        // parse entry
        auto r = parseRoot();
        if (!r) {
            m_diag.printAll();
            return false;
        }

        // dump ast logic

        return true;
    }


}