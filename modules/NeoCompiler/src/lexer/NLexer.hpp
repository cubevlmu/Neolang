#pragma once

#include <lexer/Tokens.hpp>
#include <flib/types/FList.hpp>
#include <map>

namespace neo {

    const char CHAR_EOF = 0;
    const char CHAR_CR = '\r';
    const char CHAR_LF = '\n';
    const char CHAR_SPACE = ' ';
    const char CHAR_TAB = '\t';
#ifdef __WIN32
#define NewLineChar '\n\r'
#else
#define NewLineChar '\n'
#endif

    FLIB_FORCE_INLINE bool isDigit(char ch) {
        return ch >= '0' && ch <= '9';
    }

    FLIB_FORCE_INLINE bool isHexDigit(char ch) {
        return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
    }
    
    FLIB_FORCE_INLINE bool isLetter(char ch) {
        return ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    FLIB_FORCE_INLINE i64 toNumber(char digit) {
        if (!isDigit(digit)) {
            return 9 + (digit & 15);
        }
        return digit & 15;
    }


    class NLexer
    {
    public:
        NLexer(class NSourceFile& file, flib::IAllocator& alloc);
        ~NLexer();

    public:
        bool lex();
        void debugPrint(class NDebugOutput& output);

    public:
        NToken& previousToken();
        NToken& peekPrevious();
        NToken& nextToken();
        NToken& peekNext();
        NToken& current();
        bool expectToken(NToken::TokenType);

    private:
        FLIB_FORCE_INLINE void skipSpace() {
            while (m_lex_idx < m_lex_max) {
                char c = m_src[m_lex_idx];
                if (!isspace(c)) {
                    break;
                }

                if (c == NewLineChar) {
                    m_lex_line++;
                    m_lex_cursor = 0;
                }

                m_lex_idx++;
                m_lex_cursor++;
            }
        }
        FLIB_FORCE_INLINE std::string subString(u32 idx, u32 len) {
            return idx < m_lex_max && idx + len < m_lex_max ?
                std::string{ m_src.begin() + sizeof(char) * idx, len } : "";
        }
        bool checkMatch(const flib::FStringView& token) {
            if (m_lex_idx + token.length() < m_lex_max)
                return false;
            flib::FStringView origin{ m_src.begin() + sizeof(char) * m_lex_idx, token.length() };
            return flib::equalStrings(origin, token);
        }

        FLIB_FORCE_INLINE char getChar(u32 idx) const {
            return idx > m_lex_max ? ' ' : m_src[idx];
        }

        FLIB_FORCE_INLINE void move(u32 idx) {
            m_lex_idx += idx;
            m_lex_cursor += idx;
        }

        FLIB_FORCE_INLINE void move() {
            m_lex_idx += 1;
            m_lex_cursor += 1;
        }

        FLIB_FORCE_INLINE void waitToLineEnd() {
            while (m_src[m_lex_idx] != NewLineChar) {
                move();
            }
        }

        FLIB_FORCE_INLINE void pushToken(NToken::TokenType type, const std::string& str) {
            m_tokens.push_back(NToken {
                .type = type,
                .value = str,
                .cursor = m_lex_cursor,
                .line = m_lex_line
            });
        }

        bool lexText();
        bool lexNumber();
        bool lexIdentifier();
        bool lexComment(bool doubleSlash);

        std::string getLine();

    private:
        flib::IAllocator& m_alloc;
        std::string_view m_src;
        std::vector<NToken> m_tokens;
        NSourceFile& m_source;

        u32 m_tk_idx = 0;
        u32 m_lex_line = 0;
        u32 m_lex_idx = 0;
        u32 m_lex_max = 0;
        u32 m_lex_cursor = 0;

        const static std::map<char, NToken::TokenType> s_token_types;
    };
}