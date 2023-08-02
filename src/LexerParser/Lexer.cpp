#include "Lexer.h"

namespace  JDD::Lexer {
    std::ostream &operator<<(std::ostream &flux, const JDD::Lexer::Token &token) {
        flux << "(Token: " << token.content << ", " << sAllTokenTypes[token.type] << ", " << token.line << ")";
        return flux;
    }

    std::vector<Token> JDD::Lexer::Builder::ParserTokens(std::string_view code) {
        Token current;
        std::vector<Token> tokenList;

        for (auto const& element : code) {
            switch (element) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (current.type == WHITESPACE) {
                        current.type = INT;
                        current.content.append(1, element);
                    } else if (current.type == POSSIBLE_DOUBLE) {
                        current.type = DOUBLE;
                        current.content.append(1, element);
                    } else if (current.type == IDENTIFIANT) {
                        OverToken(current, tokenList);
                        current.type = INT;
                        current.content.append(1, element);
                    } else {
                        current.content.append(1, element);
                    }
                    break;

                case '.':
                    if (current.type == INT) {
                        current.type = POSSIBLE_DOUBLE;
                        current.content.append(1, element);
                    } else if (current.type == WHITESPACE) {
                        OverToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1,element);
                        OverToken(current, tokenList);
                    } else {
                        current.content.append(1,element);
                    }
                    break;

                case '"':
                    if (current.type == WHITESPACE) {
                        current.type = POSSIBLE_STRING;
                        current.content.append(1,element);
                    } else if (current.type == POSSIBLE_STRING) {
                        current.type = STRING;
                        current.content.append(1,element);
                        OverToken(current, tokenList);
                    }
                    break;



                case '\n':
                    if (current.type != POSSIBLE_LONG_COMMENT && current.type != LONG_COMMENT) {
                        OverToken(current, tokenList);
                        current.line += 1;
                    }
                    else if (current.type == POSSIBLE_STRING || current.type == STRING) {
                        current.content.append(1, element);
                    }
                    break;

                case '/':
                    if (current.type == POSSIBLE_STRING || current.type == COMMENT) {
                        current.content.append(1, element);
                    } else if (current.type == LONG_COMMENT) {
                        current.type = COMMENT;
                        current.content.append(1, element);
                        OverToken(current, tokenList);
                    } else if (current.type == OPERATOR) {
                        current.type = COMMENT;
                        current.content.append(1, element);
                    } else {
                        OverToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1, element);
                    }
                    break;

                case '*':
                    if (current.type == OPERATOR) {
                        current.type = POSSIBLE_LONG_COMMENT;
                        current.content.append(1,element);
                    } else if (current.type == POSSIBLE_LONG_COMMENT) {
                        current.type = LONG_COMMENT;
                        current.content.append(1,element);
                    } else if (current.type != POSSIBLE_STRING) {
                        OverToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1,element);
                        OverToken(current, tokenList);
                    } else {
                        current.content.append(1,element);
                    }
                    break;

                case '+':
                case '-':
                case '{':
                case '}':
                case '[':
                case ']':
                case '(':
                case ')':
                case '=':
                case ';':
                case ',':
                case '<':
                case '>':
                    if (current.type != POSSIBLE_STRING && current.type != COMMENT) {
                        OverToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1,element);
                        OverToken(current, tokenList);
                    } else {
                        current.content.append(1,element);
                    }
                    break;

                case ' ':
                    current.content.append(1,element);
                    if (current.type != POSSIBLE_STRING && current.type != COMMENT && current.type != POSSIBLE_LONG_COMMENT) {
                        OverToken(current, tokenList);
                    }
                    break;

                default:
                    if (current.type == WHITESPACE || current.type == INT || current.type == DOUBLE) {
                        OverToken(current, tokenList);
                        current.type = IDENTIFIANT;
                        current.content.append(1, element);
                    } else {
                        current.content.append(1, element);
                    }
                    break;
            }
        }
        OverToken(current, tokenList);
        return tokenList;
    }

    void Builder::OverToken(Token &token, std::vector<Token> &tokenList) {
        if (token.type == COMMENT) {
            std::cout << "Comment found: " <<  token.content << std::endl;
        }
        else if (token.type != WHITESPACE) {
            tokenList.push_back(token);
        }

        token.type = WHITESPACE;
        token.content.erase();
    }
}