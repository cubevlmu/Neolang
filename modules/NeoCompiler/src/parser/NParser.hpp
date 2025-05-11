#pragma once

#include <common.hpp>

#include "ast/ASTType.hpp"
#include "ast/Stmts.hpp"
#include "diagnose/Diagnostic.hpp"
#include "lexer/Tokens.hpp"

#include "ast/ASTBase.hpp"
#include "ast/Decl.hpp"

namespace neo {

    namespace ast {
        class TypeCache;
    }

    struct NParserArgs
    {
        class NLexer* lexer;
        class NSourceFile* file;
        class NParsedFile& output;
        ast::TypeCache* cache;
        i32 langVer;
    };


    class NParser
    {
        struct Modifiers;
    public:
        NParser(NParserArgs args);
        ~NParser();

    public:
        bool parse();
        void debugPrint(class NDebugOutput&);

    private:
        NToken& current();
        NToken& peek();
        NToken& peekPrevious();
        NToken& advance();
        NToken& previous();
        bool match(NToken::TokenType);
        bool expect(NToken::TokenType);
        bool check(NToken::TokenType);

    private:
        Expected<void> parseRoot();
        
        Expected<ast::ASTExpr*> parseExpr();

        Expected<ast::ASTTypeNode*> parseType();

        Expected<ast::ImportStmt*> parseImport();
        Expected<ast::ModuleDecl*> parseModule();

        Expected<ast::TopLevelDecls*> parseDecls();
        Expected<ast::TopLevelDecls*> parseScopeDecls();

        Expected<ast::ClassDecl*> parseClass();
        Expected<ast::FuncDecl*> parseFunc();

        Expected<ast::FieldDecl*> parseFields();
        Expected<ast::FuncDecl*> parseFuncs();

        Expected<std::vector<ast::Attribute*>> parseAttributes();
        Expected<Modifiers> parseModifier();

        Expected<std::vector<ast::ASTExpr*>> parseFuncCallArgs();


    private:
        NParserArgs m_args;
        DiagnosticCollector m_diag;
        NLexer* m_lexer;

        static NToken::TokenType s_modifier[8];
    };
}