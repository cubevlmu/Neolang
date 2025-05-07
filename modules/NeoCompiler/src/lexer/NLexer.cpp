#include "NLexer.hpp"

#include "base/NDebugOutput.hpp"
#include "base/NSource.hpp"

namespace neo {

    const std::map<char, NToken::TokenType> NLexer::s_token_types = {
        // 算术运算符
        {'+', NToken::TokenType::kAdd},
        {'-', NToken::TokenType::kSub},
        {'*', NToken::TokenType::kMul},
        // {'/', NToken::TokenType::kDiv},
        {'%', NToken::TokenType::kMod},
    
        // 比较与移位
        {'<', NToken::TokenType::kLt},
        {'>', NToken::TokenType::kGt},
        {'=', NToken::TokenType::kAssign},
        {'!', NToken::TokenType::kLNot},
    
        // 位运算
        {'&', NToken::TokenType::kBitAnd},
        {'|', NToken::TokenType::kBitOr},
        {'^', NToken::TokenType::kBitXor},
        {'~', NToken::TokenType::kBitNot},
    
        // 括号与语法结构
        {'(', NToken::TokenType::kLParen},
        {')', NToken::TokenType::kRParen},
        {'{', NToken::TokenType::kLBraces},
        {'}', NToken::TokenType::kRBraces},
        {'[', NToken::TokenType::kLBracket},
        {']', NToken::TokenType::kRBracket},
    
        // 分隔符
        {'.', NToken::TokenType::kDot},
        {':', NToken::TokenType::kColon},
        {';', NToken::TokenType::kSemicolon},
        {',', NToken::TokenType::kComma},
    
        // 其他
        {'?', NToken::TokenType::kQuestion},
    };
    

#define LEX_TOKEN(TYPE, VALUE) NToken { .type = TYPE, .value = VALUE, .line = m_lex_line, .cursor = m_lex_cursor  }


    NLexer::NLexer(NSourceFile& file, flib::IAllocator& alloc)
        : m_alloc(alloc)
        , m_tokens {}
        , m_src{ file.getContent().cstr() }
        , m_lex_max{ (u32)m_src.length() }
        , m_source {file}
    {
    }


    NLexer::~NLexer()
    {
        m_tokens.clear();
    }


    bool NLexer::lex()
    {
        m_tokens.clear();
        m_lex_line = 1;
        m_lex_cursor = 1;

        while (m_lex_idx < m_lex_max) {
            skipSpace();

            char c = m_src[m_lex_idx];
            // lexer logic
            if (s_token_types.find(c) != s_token_types.end()) {
                NToken::TokenType type = s_token_types.at(c);
                char next = m_src[m_lex_idx + 1];
            
                if (type == NToken::TokenType::kAssign) {
                    if (next == '=') {
                        pushToken(NToken::kEq, "==");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kLNot) {
                    if (next == '=') {
                        pushToken(NToken::kNeq, "!=");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kAdd) {
                    if (next == '=') {
                        pushToken(NToken::kAddAssign, "+=");
                        goto end;
                    } else if (next == '+') {
                        pushToken(NToken::kInc, "++");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kSub) {
                    if (next == '=') {
                        pushToken(NToken::kSubAssign, "-=");
                        goto end;
                    } else if (next == '-') {
                        pushToken(NToken::kDec, "--");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kMul) {
                    if (next == '=') {
                        pushToken(NToken::kMulAssign, "*=");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kDiv) {
                    if (next == '=') {
                        pushToken(NToken::kDivAssign, "/=");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kMod) {
                    if (next == '=') {
                        pushToken(NToken::kModAssign, "%=");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kLt) {
                    if (next == '=') {
                        pushToken(NToken::kLe, "<=");
                        goto end;
                    } else if (next == '<') {
                        char third = m_src[m_lex_idx + 2];
                        if (third == '=') {
                            pushToken(NToken::kShlAssign, "<<=");
                            move();  // move extra once below
                            goto end;
                        } else {
                            pushToken(NToken::kShl, "<<");
                            goto end;
                        }
                    }
                } else if (type == NToken::TokenType::kGt) {
                    if (next == '=') {
                        pushToken(NToken::kGe, ">=");
                        goto end;
                    } else if (next == '>') {
                        char third = m_src[m_lex_idx + 2];
                        if (third == '=') {
                            pushToken(NToken::kShrAssign, ">>=");
                            move();  // move extra once below
                            goto end;
                        } else {
                            pushToken(NToken::kShr, ">>");
                            goto end;
                        }
                    }
                } else if (type == NToken::TokenType::kBitAnd) {
                    if (next == '=') {
                        pushToken(NToken::kAndAssign, "&=");
                        goto end;
                    } else if (next == '&') {
                        pushToken(NToken::kLAnd, "&&");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kBitOr) {
                    if (next == '=') {
                        pushToken(NToken::kOrAssign, "|=");
                        goto end;
                    } else if (next == '|') {
                        pushToken(NToken::kLOr, "||");
                        goto end;
                    }
                } else if (type == NToken::TokenType::kBitXor) {
                    if (next == '=') {
                        pushToken(NToken::kXorAssign, "^=");
                        goto end;
                    }
                }
            
                m_tokens.push_back(LEX_TOKEN(type, { c }));
                move();
                continue;
            end:
                move(2);
                // 额外处理 <<==、>>== 的多字符情况
                // if ((type == NToken::TokenType::kLt || type == NToken::TokenType::kGt) && next == m_src[m_lex_idx]) {
                //     // 多移动一次
                //     move();
                // }
            }            
            else if (c == '/') {
                char next = getChar(m_lex_idx + 1);
                if (next == '/') {
                    move(2);
                    auto e = lexComment(true);
                    if (!e) {
                        LogError("[Lexer] Failed to lex comment");
                    }
                }
                else if (next == '*') {
                    move(2);
                    auto e = lexComment(false);
                    if (!e) {
                        LogError("[Lexer] Failed to lex comment");
                    }
                }
                else {
                    move(1);
                    m_tokens.push_back(LEX_TOKEN(NToken::kDiv, "/"));
                }
                continue;
            }
            else if (c == '\'') {
                if (m_lex_idx + 2 < m_lex_max) {
                    char c2 = m_src[m_lex_idx + 2];
                    if (c2 == '\'') {
                        move(2);
                        m_tokens.push_back(LEX_TOKEN(NToken::kCharLit, std::string{ m_src[m_lex_idx + 1] }));
                        continue;
                    }
                }
            }
            else if (c == '\"') {
                auto txt = lexText();
                if (!txt) {
                    LogError("Failed to lex text");
                }
            }
            else {
                if (isDigit(c)) {
                    bool e = lexNumber();
                    if (!e) {
                        LogError("scan number error -> ", c);
                        LogError("[", m_lex_line, ':', m_lex_cursor, "] '", getLine(), "'");
                    }
                }
                else if (isLetter(c)) {
                    bool e = lexIdentifier();
                    if (!e) {
                        LogError("[Lexer] scan identifier error -> ", c);
                    }
                }
                else if (c == CHAR_EOF) {
                    m_tokens.push_back(LEX_TOKEN(NToken::kEOF, ""));
                    return true;
                }
                else {
                    LogError("[Lexer] unexpected symbol near -> ", c);
                    return false;
                }
                continue;
            }
        }

        return true;
    }


    void NLexer::debugPrint(NDebugOutput& output)
    {
        output.write("Lex result for file : ");
        output.writeLine(m_source.getPath());
        output.writeLine("     ");
        for (auto& tk : m_tokens) {
            output.write("\t");
            output.writeLine(tk.toString().c_str());
        }
    }


    NToken& NLexer::previousToken()
    {
        m_tk_idx--;
        if (m_tk_idx < 0) 
            m_tk_idx = 0;
        return m_tokens[m_tk_idx];
    }


    NToken& NLexer::peekPrevious()
    {
        if (m_tk_idx - 1 < 0) 
            return NToken::InvalidToken;
        return m_tokens[m_tk_idx - 1];
    }


    NToken& NLexer::nextToken()
    {
        m_tk_idx++;
        if (m_tk_idx >= m_tokens.size())
            m_tk_idx = m_tokens.size() - 1;
        return m_tokens[m_tk_idx]; 
    }


    NToken& NLexer::peekNext()
    {
        if (m_tk_idx + 1 >= m_tokens.size()) 
            return NToken::InvalidToken;
        return m_tokens[m_tk_idx + 1];
    }


    bool NLexer::expectToken(NToken::TokenType tp)
    {
        NToken& tk = peekNext();
        return tk.type == tp;
    }


    bool NLexer::lexText()
    {
        u32 start = m_lex_idx + 1;
        bool escape = false;
        char current = getChar(start);
        char next = ' ';
        std::string result{};
        move();

        while (m_lex_idx < m_lex_max) {
            current = getChar(m_lex_idx);
            if (current == 0 || current == '\r' || current == '\n') {
                char prev = getChar(m_lex_idx - 1);
                if (prev != '\\') {
                    LogError("[Lexer] expect another quotation mark before end of file or newline");
                    return false;
                }
                u32 endLine = m_lex_idx;
                auto linePart = subString(start, m_lex_idx - start);
                result.append(linePart);
                move();
                start = m_lex_idx;
                continue;
            }
            if (current == '\"') {
                auto str = subString(start, m_lex_idx - start);
                result.append(str);
                move();
                pushToken(NToken::kStringLit, result);
                return true;
            }
            move();
        }

        pushToken(NToken::kStringLit, result);
        return true;
    }


    bool NLexer::lexNumber()
    {
        u32 start = m_lex_idx;
        bool isFloat = false;

        char first = getChar(m_lex_idx);
        if (first == '0') {
            char next = getChar(m_lex_idx + 1);
            if (next == 'X' || next == 'x') {
                move(2);
                while (m_lex_idx < m_lex_max && isHexDigit(m_src[m_lex_idx]) && m_src[m_lex_idx] != NewLineChar) {
                    move();
                }
                auto hex = subString(start, m_lex_idx - start);
                pushToken(NToken::kHexLit, hex);
                return true;
            }
        }
        if (!isDigit(first)) {
            return false;
        }
        while (m_lex_idx < m_lex_max) {
            char current = m_src[m_lex_idx];
            if (!isDigit(current) || current == '\n' || current == '\r' || current == 0) {
                break;
            }
            else if (current == '.') {
                if (isFloat) {
                    return false;
                }
            }
            move();
        }
        auto number = subString(start, m_lex_idx - start);
        if (number.find('.') != std::string::npos) {
            pushToken(NToken::kFloatLit, number);
        }
        else {
            pushToken(NToken::kIntLit, number);
        }
        return true;
    }


    bool NLexer::lexIdentifier()
    {
        u32 start = m_lex_idx;

        while (m_lex_idx < m_lex_max && isLetter(m_src[m_lex_idx]) || isDigit(m_src[m_lex_idx])) {
            move();
        }

        auto idType = subString(start, m_lex_idx - start);
        auto type = NToken::checkIdentifier(idType.c_str());
        if (type != NToken::TokenType::kUnknown) {
            pushToken(type, idType);
            return true;
        }

        pushToken(NToken::kIdentifier, idType);
        return true;
    }


    bool NLexer::lexComment(bool doubleSlash)
    {
        if (doubleSlash) {
            u32 start = m_lex_idx;
            while (m_lex_idx < m_lex_max) {
                char ahead = getChar(m_lex_idx);
                if (ahead == CHAR_EOF || ahead == CHAR_CR || ahead == CHAR_LF) {
                    break;
                }
                move();
            }
           // pushToken(TokenType::CommentSingleLine, m_src.substr(start, m_lex_idx - start));
            return true;
        }
        /* */

        u32 start = m_lex_idx;
        while (m_lex_idx < m_lex_max) {
            char current = getChar(m_lex_idx);
            if (current == '*') {
                char next = getChar(m_lex_idx + 1);
                if (next == '/') {
                    move(2);
                    break;
                }
            }
            if (current == NewLineChar) {
                m_lex_line++;
                m_lex_cursor = 0;
            }
            move();
        } //TODO how about colum ? line ? fix it later
        // pushToken(TokenType::CommentMultiLine, m_src.substr(start, m_lex_idx - start));
        return true;
    }


    std::string NLexer::getLine()
    {
        u32 idx = m_lex_idx;

        while (m_lex_idx > 0) {
            m_lex_idx--;
            if (m_src[m_lex_idx] == NewLineChar) {
                break;
            }
        }
        u32 start = m_lex_idx + 1;
        m_lex_cursor = idx;

        while (m_lex_idx < m_lex_max) {
            m_lex_idx++;
            if (m_src[m_lex_idx] == NewLineChar) {
                break;
            }
        }
        auto str = subString(start, m_lex_idx - start);
        m_lex_idx = idx;
        return str;
    }

}