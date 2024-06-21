#pragma once

#include <iostream>
#include <vector>
#include <string_view>
#include <sstream>

namespace JDD::Lexer {
    enum Types {
        WHITESPACE,
        IDENTIFIANT,
        INT,
        BOOL,
        POSSIBLE_DOUBLE,
        DOUBLE,
        POSSIBLE_STRING,
        STRING,
        OPERATOR,
        COMMENT,
        POSSIBLE_LONG_COMMENT,
        LONG_COMMENT
    };

    inline constexpr const char *stringTokenType[] {
            "WHITESPACE",
            "IDENTIFIANT",
            "INT",
            "BOOL",
            "POSSIBLE_DOUBLE",
            "DOUBLE",
            "POSSIBLE_STRING",
            "STRING",
            "OPERATOR",
            "COMMENT",
            "POSSIBLE_LONG_COMMENT",
            "LONG_COMMENT"
    };

    class Token {
    public:
        enum Types type = WHITESPACE;
        std::string content;
        int line = 1;

        friend std::ostream& operator<<(std::ostream& flux, Token const& token);
    };

    std::vector<Token> Lexer(std::string_view code);
    void ConcludeToken(Token& token, std::vector<Token>& tokenList);
    void appendElement(Token& token, char &e);
}