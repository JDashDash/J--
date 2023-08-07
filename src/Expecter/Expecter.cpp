#include "Expecter.h"
#include "../Modules/ModuleManager.h"

std::optional<JDD::Lexer::Token> ExpectIdentifiant(std::vector<JDD::Lexer::Token>::const_iterator& current) {
    if (current->type == JDD::Lexer::IDENTIFIANT) {
        return *(current++);
    }
    return std::nullopt;
}

std::optional<JDD::Lexer::Token> ExpectOperator(std::vector<JDD::Lexer::Token>::const_iterator& current, std::string_view o) {
    if (current->type == JDD::Lexer::OPERATOR && o.empty()) {
        return *(current++);
    } else if (current->type == JDD::Lexer::OPERATOR && !o.empty() && current->content == o) {
        return *(current++);
    }
    return std::nullopt;
}

std::optional<std::string> ExpectConditionalOperator(std::vector<JDD::Lexer::Token>::const_iterator& current) {
    if (ExpectOperator(current, ">").has_value()) {
        if (ExpectOperator(current, "=").has_value())
            return ">=";
        return ">";
    } else if (ExpectOperator(current, "<").has_value()) {
        if (ExpectOperator(current, "=").has_value())
            return "<=";
        return "<";
    } else if (ExpectOperator(current, "!").has_value() && ExpectOperator(current, "=").has_value()) {
        return "!=";
    } else if (ExpectOperator(current, "=").has_value() && ExpectOperator(current, "=").has_value()) {
        return "==";
    }
    return std::nullopt;
}

std::optional<JDD::Definition::Types> ExpectType(std::vector<JDD::Lexer::Token>::const_iterator& current) {
    if (current->type == JDD::Lexer::IDENTIFIANT) {
        if (current->content == "int") {
            current++;
            return JDD::Definition::Types::INT;
        } else if (current->content == "double") {
            current++;
            return JDD::Definition::Types::DOUBLE;
        } else if (current->content == "string") {
            current++;
            return JDD::Definition::Types::STRING;
        } else if (current->content == "boolean") {
            current++;
            return JDD::Definition::Types::BOOLEAN;
        } else if (current->content == "final") {
            current++;
            return JDD::Definition::Types::FINAL_NotType;
        } else if (current->content == "void") {
            current++;
            return JDD::Definition::Types::VOID;
        }
    }
    return std::nullopt;
}

std::optional<JDD::Definition::Value> ExpectValue(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data) {
    if (current->type == JDD::Lexer::INT || current->type == JDD::Lexer::DOUBLE || current->type == JDD::Lexer::STRING ||
        (current->type == JDD::Lexer::IDENTIFIANT && (current->content == "true" || current->content == "false"))) {
        JDD::Definition::Value v;
        v.content = current->content;
        if (current->type == JDD::Lexer::STRING) {
            v.type = JDD::Definition::Types::STRING;
        } else if (current->type == JDD::Lexer::INT) {
            v.type = JDD::Definition::Types::INT;
        } else if (current->type == JDD::Lexer::DOUBLE) {
            v.type = JDD::Definition::Types::DOUBLE;
        } else if (current->type == JDD::Lexer::IDENTIFIANT) {
            v.type = JDD::Definition::Types::BOOLEAN;
        }
        current++;

        if (v.type == JDD::Definition::Types::STRING) {
            while (ExpectOperator(current, ".").has_value() && data.isStringModuleImported) {
                JDD::Modules::ModulesManager::useStringModule(current, data, v);
            }
        } else if (v.type == JDD::Definition::Types::BOOLEAN) {
            while (ExpectOperator(current, ".").has_value() && data.isBooleanModuleImported) {
                JDD::Modules::ModulesManager::useBooleanModule(current, data, v);
            }
        }

        return v;
    } else if (current->type == JDD::Lexer::IDENTIFIANT) {
        if (data.isVariable(current->content)) { // Variable
            auto var = data.getVariable(current->content);
            JDD::Definition::Value v = var->value;
            current++;
            if (var->type == JDD::Definition::STRING) {
                v.type = JDD::Definition::Types::STRING;
                while (ExpectOperator(current, ".").has_value() && data.isStringModuleImported)
                    JDD::Modules::ModulesManager::useStringModule(current, data, v);
            } else if (var->type == JDD::Definition::Types::BOOLEAN) {
                v.type = JDD::Definition::Types::BOOLEAN;
                while (ExpectOperator(current, ".").has_value() && data.isBooleanModuleImported)
                    JDD::Modules::ModulesManager::useBooleanModule(current, data, v);
            }
            return v;
        } else if (current->content == "String") { // String Module without any value (variable value or classic value)
            JDD::Definition::Value v;
            v.content = "";
            v.type = JDD::Definition::Types::STRING;
            current++;
            while (ExpectOperator(current, ".").has_value() && data.isStringModuleImported) {
                JDD::Modules::ModulesManager::useStringModule(current, data, v);
            }
            return v;
        } else if (current->content == "Boolean") { // Boolean Module without any value (variable value or classic value)
            JDD::Definition::Value v;
            v.content = "";
            v.type = JDD::Definition::Types::BOOLEAN;
            current++;
            while (ExpectOperator(current, ".").has_value() && data.isBooleanModuleImported) {
                JDD::Modules::ModulesManager::useBooleanModule(current, data, v);
            }
            return v;
        }
    }

    return std::nullopt;
}
