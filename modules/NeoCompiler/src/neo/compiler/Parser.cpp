#include "Parser.hpp"

#include "neo/ast/Type.hpp"
#include "neo/ast/Decl.hpp"
#include "neo/base/StringUtils.hpp"
#include "neo/diagnose/Diagnostic.hpp"
#include "neo/compiler/Lexer.hpp"
#include "neo/compiler/Tokens.hpp"
#include "ParsedFile.hpp"
#include "neo/ast/Stmts.hpp"

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

        void applyTo(FuncDecl* f);
        void applyTo(VarDecl* c);
        void applyTo(FieldDecl* f);
    };


    TokenType NParser::s_modifier[] = {
        TokenType::kInline,
        TokenType::kStatic,
        TokenType::kConst,
        TokenType::kExport,
        TokenType::kInternal,
        TokenType::kProtected,
        TokenType::kPrivate,
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
    bool NParser::match(TokenType type)
    {
        return current().type == type;
    }
    bool NParser::expect(TokenType type)
    {
        return m_lexer->expectToken(type);
    }
    bool NParser::check(TokenType type)
    {
        return m_lexer->current().type == type;
    }


    Expected<void> NParser::parseRoot()
    {
        auto& output = m_args.output;

        while (!m_lexer->expectToken(TokenType::kEOF)) {
            if (check(TokenType::kImport)) {
                auto p_import_Ret = parseImport();
                CHECK_ERROR(p_import_Ret);
                output.Nodes.push_back(p_import_Ret.value());
            }
            else if (check(TokenType::kModule)) {
                auto p_module_Ret = parseModule();
                CHECK_ERROR(p_module_Ret);
                output.Nodes.push_back(p_module_Ret.value());
            }
            // else if (check(TokenType::kClass) || check(TokenType::kStruct)) {
            //     if (!parseClass()) 
            //         return false;
            // }
            // else if (check(TokenType::kInterface)) {

            // }
            // else if (check(TokenType::kFun)) {
            //     if (!parseFunction()) return false;
            // }
            // else if (check(TokenType::kVar) || check(TokenType::kVal)) {
            //     if (!parseVariableDecl()) return false;
            // }
            else {
                return Error::failure("Unexpected token at top level.", ERRR());
            }
        }

        return Error::success();
    }


    //TODO fix type parse
    Expected<ASTTypeNode*> NParser::parseType()
    {
        if (!check(TokenType::kIdentifier)) {
            return nullptr;
        }
        std::string name = current().value;

        if (expect(TokenType::kLBracket)) {
            advance();
            if (!expect(TokenType::kRBracket)) {
                return Error::failure(msg("Array type need close bracket ']' but got '", peek().value, "'"), ERRR());
            }
            advance();
            return new ASTTypeNode("", name);
        }

    }

    // tested
    Expected<ImportStmt*> NParser::parseImport()
    {
        if (!check(TokenType::kImport)) {
            return Error::failure(msg("unexpected token '", current().typeString(), "' for import expression"), &m_diag, current(), m_args.file);
        }
        std::stringstream moduleNameBuilder{};

        do {
            advance();
            if (check(TokenType::kIdentifier)) {
                moduleNameBuilder << current().value;
            }
            else if (check(TokenType::kDot)) {
                moduleNameBuilder << '.';
            }
            else {
                return Error::failure(msg("unexpected token '", current().typeString(), "' for module name"), ERRR());
            }
        } while (expect(TokenType::kSemicolon));

        return new ImportStmt(moduleNameBuilder.str());
    }


    Expected<ModuleDecl*> NParser::parseModule()
    {
        if (!check(TokenType::kModule)) {
            return Error::failure(msg("unexpected token '", current().typeString(), "' for import expression"), ERRR());
        }
        std::string module{};

        do {
            advance();
            if (check(TokenType::kIdentifier)) {
                module = current().value;
            }
            else {
                return Error::failure(msg("unexpected token '", current().typeString(), "' for module declear"), ERRR());
            }
        } while (expect(TokenType::kSemicolon) || expect(TokenType::kLBraces));

        if (expect(TokenType::kSemicolon)) {
            auto r = parseDecls();
            if (!r) {
                delete r.value();
                return r.error();
            }
            auto ptr = new ModuleDecl(module, nullptr);
            ptr->children = r.value();
            return ptr;
        }
        else if (expect(TokenType::kLBraces)) {
            auto r = parseScopeDecls();
            if (!r) {
                delete r.value();
                return r.error();
            }
            auto ptr = new ModuleDecl(module, nullptr);
            ptr->children = r.value();
            return ptr;
        }
        else {
            return Error::failure(msg("expect ';' or '{' behind module declear statement, but found '", current().typeString(), "'"), ERRR());
        }
    }


    Expected<TopLevelDecls*> NParser::parseDecls()
    {
        ScopeGuard dC{ new TopLevelDecls {} };

        // logic
        if (check(TokenType::kModule)) {
            auto p_module_Ret = parseModule();
            CHECK_ERROR(p_module_Ret);
            dC->decls.push_back(p_module_Ret.value());
        }
        else if (check(TokenType::kClass)) {
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


    Expected<TopLevelDecls*> NParser::parseScopeDecls()
    {
        if (!check(TokenType::kLBraces)) {
            return Error::failure("trying parse declear scope but not in a scope", ERRR());
        }
        advance();

        auto r = parseDecls();
        if (!r) {
            return r.error();
        }

        if (!expect(TokenType::kRBraces)) {
            return Error::failure(msg("scope not closed, expect '}' but found : '", current().typeString(), "'"), ERRR());
        }
        advance();

        return r.value();
    }


    Expected<ClassDecl*> NParser::parseClass()
    {
        if (!check(TokenType::kIdentifier)) {
            return Error::failure(msg("expected identifier for class name but got : '", current().typeString(), "'"), ERRR());
        }
        std::string& name = current().value;
        std::vector<ASTTypeNode*> baseClasses{};
        ScopeGuard g{ new ClassDecl(name.c_str(), baseClasses) };


        if (check(TokenType::kColon)) {
            if (!expect(TokenType::kIdentifier)) {
                return Error::failure(msg("unexpect token '", current().typeString(), "' for base classes"), ERRR());
            }
            u32 commaCount = 0;
            do {
                advance();
                if (check(TokenType::kIdentifier)) {
                    if (commaCount != baseClasses.size()) {
                        return Error::failure("comma count not match base class defines", ERRR());
                    }
                    g->baseClasses.push_back(new ASTTypeNode("", current().value));
                }
                else if (check(TokenType::kComma)) {
                    commaCount++;
                }
                else {
                    return Error::failure(msg("unexpect token '", current().typeString(), "' for base classes"), ERRR());
                }
            } while (expect(TokenType::kLBraces) || expect(TokenType::kSemicolon));
            advance();

            if (check(TokenType::kLBraces)) {
                goto parseBody;
            }
            else if (check(TokenType::kSemicolon)) {
                goto end;
            }
            else {
                return Error::failure(msg("unexpect token '", current().typeString(), "' for class declear"), ERRR());
            }
        }
        else if (check(TokenType::kLBraces)) {
            goto parseBody;
        }
        else if (check(TokenType::kSemicolon)) {
            goto end;
        }
        else {
            return Error::failure(msg("unexpect token '", current().typeString(), "' for class declear"), ERRR());
        }

    parseBody:

        do {
            advance();

            // for class body parsing
            std::vector<Attribute*> c_atts;
            Modifiers mf{};

            // todo
            if (check(TokenType::kLBracket)) {
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

            if (check(TokenType::kCtor)) {

            }
            else if (check(TokenType::kDtor)) {

            }
            else if (check(TokenType::kFun)) {

            }
            else if (check(TokenType::kField)) {

            }
            else if (check(TokenType::kVal) || check(TokenType::kVar)) {

            }
            else if (check(TokenType::kClass)) {
                auto r = parseClass();
                CHECK_ERROR(r);
                g->subClasses.push_back(r.value());
            }
            else if (check(TokenType::kStruct)) {

            }
            else if (check(TokenType::kInterface)) {

            }
            else {
                return Error::failure(msg("unexpected token '", current().value, "' for class member"), ERRR());
            }
        } while (expect(TokenType::kRBraces));

    end:
        advance();
        return g.getPtr();
        // if (!check(TokenType::kClass)) {
        //     return Error::failure("", ERRR());
        // }
        // advance();

        // ScopeGuard g {new ClassDecl(const std::string_view &name, std::vector<ASTTypeRef> baseClasses)};
    }


    Expected<FuncDecl*> NParser::parseFunc()
    {
        if (!check(TokenType::kFun) && expect(TokenType::kIdentifier)) {
            return Error::failure(msg("unexpected token for function declear : ", current().value, " ", peek().value), ERRR());
        }
        advance();
        std::string name = current().value;

        if (!expect(TokenType::kLParen)) {
            return Error::failure(msg("function declear expect '(' for function arguments but got '", peek().value, "'"), ERRR());
        }
        advance();
        std::vector<VarDecl*> args{};

//TODO implement it!        do {
//            advance();
//            if (check(TokenType::kIdentifier) && expect(TokenType::kColon)) {
//                std::string& arg_name = current().value;
//                advance();
//                if (!expect(TokenType::kIdentifier)) {
//                    advance();
//                    return Error::failure(msg("expect type identifier for function argument, but receive '", current().value, "'"), ERRR());
//                }
//                advance();
//                auto* type = new ASTTypeNode ("", current().value);
//                args.push_back(new VarDecl());
//                //TODO
//            }
//        } while ();

    }


    Expected<std::vector<Attribute*>> NParser::parseAttributes()
    {
        std::vector<Attribute*> attrs{};

        do {
            if (!check(TokenType::kLBracket) && expect(TokenType::kIdentifier)) {
                return attrs;
            }
            advance();

            std::string name = current().value;
            ScopeGuard g{ new Attribute {} };
            g->name = name;

            if (!expect(TokenType::kLParen) && !expect(TokenType::kRBracket)) {
                return attrs;
            }
            advance();
            if (check(TokenType::kLParen)) {
                auto r = parseFuncCallArgs();
                CHECK_ERROR(r);
                g->arguments.swap(r.value());
                if (!expect(TokenType::kRBracket)) {
                    CLEARUP(attrs);
                    return Error::failure(msg("expect ']' to close attribute attach but got '", current().value, "'"), ERRR());
                }
                attrs.push_back(g.getPtr());
            }
            else if (check(TokenType::kRBracket)) {
                attrs.push_back(g.getPtr());
            }
            else {
                CLEARUP(attrs);
                return Error::failure(msg("unexpect token '", current().value, "' after attribute attach's name"), ERRR());
            }
        } while (expect(TokenType::kLBracket));

        return attrs;
    }


    Expected<NParser::Modifiers> NParser::parseModifier()
    {
        Modifiers mf{};
        previous();

        do {
            advance();
            if (check(TokenType::kPrivate)) {
                mf.isPrivate = true;
            }
            else if (check(TokenType::kProtected)) {
                mf.isProtected = true;
            }
            else if (check(TokenType::kInternal))
            {
                mf.isInternal = true;
            }
            else if (check(TokenType::kInline))
            {
                mf.isInline = true;
            }
            else if (check(TokenType::kStatic))
            {
                mf.isStatic = true;
            }
            else if (check(TokenType::kConst))
            {
                mf.isConst = true;
            }
            else {
                return mf;
            }
        } while (std::find(&s_modifier[0], &s_modifier[7], peek().type));

        return mf;
    }


    Expected<std::vector<ASTExpr*>> NParser::parseFuncCallArgs()
    {
        std::vector<ASTExpr*> args{};
        if (!check(TokenType::kLParen)) {
            return args;
        }

        do {
            advance();
            auto r = parseExpr();
            if (!r) {
                return r.error();
            }
            args.push_back(r.value());
        } while (expect(TokenType::kRParen));

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


    Expected<ASTExpr *> NParser::parseExpr() {
        return Expected<ASTExpr *>(nullptr);
    }


}