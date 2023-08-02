#include "Expecter.h"

std::optional<JDD::Lexer::Token> ExpectIdentifiant(std::vector<JDD::Lexer::Token>::const_iterator& current) {
    if (current->type == JDD::Lexer::IDENTIFIANT) {
        return *(current++);
    }
    return std::nullopt;
}

std::optional<JDD::Lexer::Token> ExpectOperator(std::vector<JDD::Lexer::Token>::const_iterator& current, std::string_view o ) {
    if (current->type == JDD::Lexer::OPERATOR || (current->type == JDD::Lexer::OPERATOR && current->content == o )) {
        return *(current++);
    }
    return std::nullopt;
}

std::optional<JDD::Definition::Types> ExpectType(std::vector<JDD::Lexer::Token>::const_iterator& current) {
    switch (current->type) {
        case JDD::Lexer::INT:
            return JDD::Definition::Types::INT;
        case JDD::Lexer::DOUBLE:
            return JDD::Definition::Types::DOUBLE;
        case JDD::Lexer::STRING:
            return JDD::Definition::Types::STRING;
        case JDD::Lexer::IDENTIFIANT:
            if (current->content == "true" || current->content == "false") {
                return JDD::Definition::Types::BOOLEAN;
            }
            break;
        default:
            break;
    }
    return std::nullopt;
}

std::optional<JDD::Definition::Value> ExpectValue(std::vector<JDD::Lexer::Token>::const_iterator& current) {
    if (current->type == JDD::Lexer::INT || current->type == JDD::Lexer::DOUBLE || current->type == JDD::Lexer::STRING ||
        (current->type == JDD::Lexer::IDENTIFIANT && (current->content == "true" || current->content == "false"))) {

        JDD::Definition::Value v;
        v.content = current->content;

        if (current->type == JDD::Lexer::STRING) {
            v.type = JDD::Definition::Types::STRING;
            v.content = v.content.substr(1, v.content.length() - 2); // Remove quotes
        } else if (current->type == JDD::Lexer::INT) {
            v.type = JDD::Definition::Types::INT;
        } else if (current->type == JDD::Lexer::DOUBLE) {
            v.type = JDD::Definition::Types::DOUBLE;
        } else if (current->type == JDD::Lexer::IDENTIFIANT) {
            v.type = JDD::Definition::Types::BOOLEAN;
        }

        ++current;
        return v;
    }

    return std::nullopt;
}