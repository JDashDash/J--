#include "Lexer.h"

namespace JDD::Lexer {
    std::ostream &operator<<(std::ostream &flux, const Token &token) {
        flux << " [TOKEN OBJECT] >>> " << token.content << " | " << stringTokenType[token.type] << " | " << token.line;
        return flux;
    }

    std::string parseString(const std::string& content) {
        std::stringstream result;
        size_t pos = 1;
        while (pos < content.size() - 1) {
            if (content[pos] == '\\') {
                switch (content[pos + 1]) {
                    case 'n':
                        result << '\n';
                        break;
                    case 't':
                        result << '\t';
                        break;
                    case '\\':
                        result << '\\';
                        break;
                    case '"':
                        result << '"';
                        break;
                    default:
                        break;
                }
                pos += 2;
            } else {
                result << content[pos];
                pos++;
            }
        }
        return result.str();
    }


    std::vector<Token> Lexer(std::string_view code) {
        std::vector<Token> tokenList;
        Token current;

        for (auto& e : code) {
            switch (e) {
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
                        current.content.append(1, e);
                    } else if (current.type == POSSIBLE_DOUBLE) {
                        current.type = DOUBLE;
                        current.content.append(1, e);
                    } else {
                        current.content.append(1, e);
                    }
                    break;

                case '.':
                    if (current.type == INT) {
                        current.type = POSSIBLE_DOUBLE;
                        current.content.append(1, e);
                    } else {
                        ConcludeToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1, e);
                        ConcludeToken(current, tokenList);
                    }
                    break;

                case '/':
                    if (current.type == POSSIBLE_STRING || current.type == COMMENT || current.type == POSSIBLE_LONG_COMMENT) {
                        current.content.append(1, e);
                    } else if (current.type == LONG_COMMENT) {
                        current.type = COMMENT;
                        current.content.append(1, e);
                        ConcludeToken(current, tokenList);
                    } else if (current.type == OPERATOR) {
                        current.type = COMMENT;
                        current.content.append(1, e);
                    } else {
                        ConcludeToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1, e);
                    }
                    break;

                case '*':
                    if (current.type == OPERATOR) {
                        current.type = POSSIBLE_LONG_COMMENT;
                        current.content.append(1,e);
                    } else if (current.type == POSSIBLE_LONG_COMMENT) {
                        current.type = LONG_COMMENT;
                        current.content.append(1,e);
                    } else if (current.type != POSSIBLE_STRING) {
                        ConcludeToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1,e);
                        ConcludeToken(current, tokenList);
                    } else {
                        current.content.append(1,e);
                    }
                    break;

                case '"':
                    if (current.type == WHITESPACE) {
                        current.type = POSSIBLE_STRING;
                        current.content.append(1,e);
                    } else if (current.type == POSSIBLE_STRING) {
                        current.type = STRING;
                        current.content.append(1,e);
                        current.content = parseString(current.content);
                        ConcludeToken(current, tokenList);
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
                case ':':
                    if (current.type != POSSIBLE_STRING && current.type != COMMENT) {
                        ConcludeToken(current, tokenList);
                        current.type = OPERATOR;
                        current.content.append(1,e);
                        ConcludeToken(current, tokenList);
                    } else {
                        current.content.append(1,e);
                    }
                    break;

                case ' ':
                    if (current.type == WHITESPACE) {
                        continue;
                    } else if (current.type != POSSIBLE_STRING && current.type != COMMENT && current.type != POSSIBLE_LONG_COMMENT) {
                        ConcludeToken(current, tokenList);
                    } else {
                        current.content.append(1, e);
                    }
                    break;

                case '\n':
                    if (current.type == POSSIBLE_STRING || current.type == STRING) {
                        current.content.append(1, e);
                    } else if (current.type != POSSIBLE_LONG_COMMENT && current.type != LONG_COMMENT) {
                        ConcludeToken(current, tokenList);
                    }
                    current.line += 1;
                    break;

                default:
                    if (current.type == WHITESPACE || current.type == INT || current.type == DOUBLE) {
                        ConcludeToken(current, tokenList);
                        current.type = IDENTIFIANT;
                        current.content.append(1, e);
                    } else {
                        current.content.append(1, e);
                    }
                    break;
            }
        }
        ConcludeToken(current, tokenList);

        for (auto& t : tokenList) {
            if ((t.content == "true" || t.content == "false") && t.type == IDENTIFIANT) {
                t.type = BOOL;
            }
        }

        return tokenList;
    }

    void ConcludeToken(Token &token, std::vector<Token> &tokenList) {
        if (token.type == COMMENT || token.type == LONG_COMMENT) {
            std::cout << "COMMENT >>> " << token << std::endl;
        }

        if (token.type != WHITESPACE && token.type != COMMENT && token.type != LONG_COMMENT) {
            tokenList.push_back(token);
        }

        token.type = WHITESPACE;
        token.content.erase();
    }
}