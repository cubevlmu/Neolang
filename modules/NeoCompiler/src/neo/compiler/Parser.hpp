#pragma once

#include "neo/common.hpp"

#include "neo/ast/Type.hpp"
#include "neo/ast/Stmts.hpp"
#include "neo/diagnose/Diagnostic.hpp"
#include "neo/compiler/Tokens.hpp"

#include "neo/ast/Base.hpp"
#include "neo/ast/Decl.hpp"

namespace neo {

    struct NParserArgs
    {
        class NLexer* lexer;
        class NSourceFile* file;
        class NParsedFile& output;
        i32 langVer;
    };


#define APPLY_MODIFIER(V, MD) do { \
        V->modifier = MD.value();     \
   } while(false)
#define APPLY_MODIFIER_RAW(V, MD) do { \
        V->modifier = std::move(MD);   \
        MD = ASTModifier {};           \
   } while(false)
#define APPLY_ATTRIBUTES(V, AT) do { \
        V->attributes = std::move(AT); \
        AT = std::vector<Attribute*>();\
    } while(false)


    class NParser
    {
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
        bool match(TokenType);
        bool expect(TokenType);
        bool check(TokenType);

    private:
        Expected<void> parseRoot();
        
        Expected<ASTExpr*> parseExpr();

        Expected<ASTTypeNode*> parseType();

        Expected<ImportStmt*> parseImport();
        Expected<ModuleDecl*> parseModule();

        Expected<ASTDecl*> parseDecl();
        Expected<TopLevelDecls*> parseScopeDecls();

        Expected<ClassDecl*> parseClass();
        Expected<EnumDecl*> parseEnum();
        Expected<InterfaceDecl*> parseInterface();
        Expected<StructDecl*> parseStruct();
        Expected<FuncDecl*> parseFunc();
        Expected<VarDecl*> parseVarDecl();

        Expected<FieldDecl*> parseField();

        Expected<std::vector<Attribute*>> parseAttributes();
        Expected<ASTModifier> parseModifier();

        Expected<std::vector<ASTExpr*>> parseFuncCallArgs();


    private:
        NParserArgs m_args;
        DiagnosticCollector m_diag;
        NLexer* m_lexer;

        static TokenType s_modifier[8];
    };
}