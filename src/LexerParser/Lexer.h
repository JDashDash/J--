#pragma once

#include <iostream>
#include <vector>
#include <string_view>

namespace JDD::Lexer {
    enum Types {
        WHITESPACE,
        IDENTIFIANT,
        INT,
        POSSIBLE_DOUBLE,
        DOUBLE,
        POSSIBLE_STRING,
        STRING,
        OPERATOR,
        POSSIBLE_COMMENT,
        COMMENT
    };

    inline constexpr const char *sAllTokenTypes[] {
            "WHITESPACE",
            "IDENTIFIANT",
            "INT",
            "POSSIBLE_DOUBLE",
            "DOUBLE",
            "POSSIBLE_STRING",
            "STRING",
            "OPERATOR",
            "POSSIBLE_COMMENT",
            "COMMENT"
    };

    class Token {
    public:
        enum Types type = WHITESPACE;
        std::string content;
        int line = 1;

        friend std::ostream& operator<<(std::ostream& flux, Token const& token);
    };

    class Builder {
    public:
        static std::vector<Token> ParserTokens(std::string_view code);
        static void OverToken(Token& token, std::vector<Token>& tokenList);
    };
}