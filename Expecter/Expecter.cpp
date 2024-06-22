#include "Expecter.h"

namespace JDD::Parser {
    std::optional<JDD::Lexer::Token> ExpectIdentifiant(std::vector<JDD::Lexer::Token>::iterator& current)  {
        if (current->type == JDD::Lexer::IDENTIFIANT) {
            return *(current++);
        }
        return std::nullopt;
    }

    std::optional<JDD::Definitions::Value> ExpectValue(std::vector<JDD::Lexer::Token>::iterator& current) {
        if (current->type == Lexer::INT) {
            JDD::Definitions::Value v;
            v.content = current->content;
            v.type = JDD::Definitions::Types::INT;
            current++;
            return v;
        } else if (current->type == Lexer::DOUBLE) {
            JDD::Definitions::Value v;
            v.content = current->content;
            v.type = JDD::Definitions::Types::DOUBLE;
            current++;
            return v;
        } if (current->type == Lexer::STRING) {
            JDD::Definitions::Value v;
            v.content = current->content;
            v.type = JDD::Definitions::Types::STRING;
            current++;
            return v;
        } if (current->type == Lexer::BOOL) {
            JDD::Definitions::Value v;
            v.content = current->content;
            v.type = JDD::Definitions::Types::BOOLEAN;
            current++;
            return v;
        }
        return std::nullopt;
    }

    std::optional<JDD::Definitions::Types> ExpectType(std::vector<JDD::Lexer::Token>::iterator& current) {
        if (current->type == JDD::Lexer::IDENTIFIANT) {
            if (current->content == "int") {
                current++;
                return JDD::Definitions::Types::INT;
            } else if (current->content == "double") {
                current++;
                return JDD::Definitions::Types::DOUBLE;
            } else if (current->content == "string") {
                current++;
                return JDD::Definitions::Types::STRING;
            } else if (current->content == "boolean") {
                current++;
                return JDD::Definitions::Types::BOOLEAN;
            } else if (current->content == "final") {
                current++;
                return JDD::Definitions::Types::FINAL_NotType;
            } else if (current->content == "void") {
                current++;
                return JDD::Definitions::Types::VOID;
            }
        }
        return std::nullopt;
    }

    std::optional<JDD::Lexer::Token> ExpectOperator(std::vector<JDD::Lexer::Token>::iterator& current, std::string_view o) {
        if ((current->type == JDD::Lexer::OPERATOR && o.empty())
                || (current->type == JDD::Lexer::OPERATOR && current->content == o)) {
            return *(current++);
        }
        return std::nullopt;
    }

    std::optional<std::string> ExpectConditionalOperator(std::vector<JDD::Lexer::Token>::iterator& current) {
        if (ExpectOperator(current, ">").has_value()) {
            if (ExpectOperator(current, "=").has_value()) {
                return ">=";
            }
            return ">";
        } else if (ExpectOperator(current, "<").has_value()) {
            if (ExpectOperator(current, "=").has_value()) {
                return "<=";
            }
            return "<";
        } else if (ExpectOperator(current, "=").has_value() && ExpectOperator(current, "=").has_value()) {
            return "==";
        } else if (ExpectOperator( current, "!").has_value() && ExpectOperator(current, "=").has_value()) {
            return "!=";
        }
        return std::nullopt;
    }



    // Math or classic value : numbers (decimal + int), strings

    std::pair<bool, int> haveChar(const std::vector<JDD::Lexer::Token>& list, std::string_view element) {
        for (int i = 0; i < list.size();i++) {
            if (list[i].type == Lexer::OPERATOR && list[i].content == element) {
                return {true, i}; // return if there is a "*", "/", "+", "-" and the position of the char
            }
        }
        return {false, 0};
    }

    std::pair<JDD::Lexer::Token, JDD::Lexer::Token> getElementsFromIndexChar(const std::vector<JDD::Lexer::Token>& list, int indexChar) {
        JDD::Lexer::Token leftElement = list[indexChar-1];
        JDD::Lexer::Token rightElement = list[indexChar+1];
        return {leftElement, rightElement};
    }

    JDD::Definitions::Value ReturnFinalValueFromListToken(JDD::Definitions::Data& data, std::vector<JDD::Lexer::Token>::iterator& current, std::vector<JDD::Lexer::Token>& list, JDD::Definitions::Types& awaitingType, std::optional<JDD::Definitions::Function>& function) {
        JDD::Definitions::Value value;
        if (list.size() > 1 && (awaitingType == Definitions::INT || awaitingType == Definitions::VOID)) {
            auto possibleMul = haveChar(list, "*");
            while (possibleMul.first) {
                auto elements = getElementsFromIndexChar(list, possibleMul.second);
                int left = 0;
                int right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && data.getVariableFromName(elements.first.content)->type == Definitions::INT) {
                    left = std::stoi(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stoi(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && data.getVariableFromName(elements.second.content)->type == Definitions::INT) {
                    right = std::stoi(data.getVariableFromName(elements.second.content)->value.content);
                } else {
                    right = std::stoi(elements.second.content);
                }

                list[possibleMul.second].content = std::to_string(left * right);
                list[possibleMul.second].type == Lexer::INT;

                if (possibleMul.second - 1 >= 0 && possibleMul.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleMul.second - 1);
                    list.erase(list.begin() + possibleMul.second);
                }

                possibleMul = haveChar(list, "*");
            }

            auto possibleDiv = haveChar(list, "/");
            while (possibleDiv.first) {
                auto elements = getElementsFromIndexChar(list, possibleDiv.second);
                int left = 0;
                int right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && data.getVariableFromName(elements.first.content)->type == Definitions::INT) {
                    left = std::stoi(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stoi(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && data.getVariableFromName(elements.second.content)->type == Definitions::INT) {
                    right = std::stoi(data.getVariableFromName(elements.second.content)->value.content);
                } else {
                    right = std::stoi(elements.second.content);
                }

                list[possibleDiv.second].content = std::to_string(left / right);
                list[possibleDiv.second].type == Lexer::INT;

                if (possibleDiv.second - 1 >= 0 && possibleDiv.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleDiv.second - 1);
                    list.erase(list.begin() + possibleDiv.second);
                }
                possibleDiv = haveChar(list, "/");
            }

            auto possibleAdd = haveChar(list, "+");
            while (possibleAdd.first) {
                auto elements = getElementsFromIndexChar(list, possibleAdd.second);
                int left = 0;
                int right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && data.getVariableFromName(elements.first.content)->type == Definitions::INT) {
                    left = std::stoi(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stoi(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && data.getVariableFromName(elements.second.content)->type == Definitions::INT) {
                    right = std::stoi(data.getVariableFromName(elements.second.content)->value.content);
                } else {
                    right = std::stoi(elements.second.content);
                }

                list[possibleAdd.second].content = std::to_string(left + right);
                list[possibleAdd.second].type == Lexer::INT;

                if (possibleAdd.second - 1 >= 0 && possibleAdd.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleAdd.second - 1);
                    list.erase(list.begin() + possibleAdd.second);
                }

                possibleAdd = haveChar(list, "+");
            }

            auto possibleSub = haveChar(list, "-");
            while (possibleSub.first) {
                auto elements = getElementsFromIndexChar(list, possibleSub.second);
                int left = 0;
                int right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && data.getVariableFromName(elements.first.content)->type == Definitions::INT) {
                    left = std::stoi(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stoi(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && data.getVariableFromName(elements.second.content)->type == Definitions::INT) {
                    right = std::stoi(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    right = std::stoi(elements.second.content);
                }

                list[possibleSub.second].content = std::to_string(left - right);
                list[possibleSub.second].type == Lexer::INT;

                if (possibleSub.second - 1 >= 0 && possibleSub.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleSub.second - 1);
                    list.erase(list.begin() + possibleSub.second);
                }

                possibleSub = haveChar(list, "-");
            }

            value.type = Definitions::INT;
            value.content = list[list.size()-1].content;
        }
        else if (list.size() > 1 && (awaitingType == Definitions::DOUBLE || awaitingType == Definitions::VOID)) {
            auto possibleMul = haveChar(list, "*");
            while (possibleMul.first) {
                auto elements = getElementsFromIndexChar(list, possibleMul.second);
                double left = 0;
                double right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    left = std::stod(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stod(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    right = std::stod(data.getVariableFromName(elements.second.content)->value.content);
                } else {
                    right = std::stod(elements.second.content);
                }

                list[possibleMul.second].content = std::to_string(left * right);
                list[possibleMul.second].type == Lexer::DOUBLE;

                if (possibleMul.second - 1 >= 0 && possibleMul.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleMul.second - 1);
                    list.erase(list.begin() + possibleMul.second);
                }

                possibleMul = haveChar(list, "*");
            }

            auto possibleDiv = haveChar(list, "/");
            while (possibleDiv.first) {
                auto elements = getElementsFromIndexChar(list, possibleDiv.second);
                double left = 0;
                double right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    left = std::stod(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stod(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    right = std::stod(data.getVariableFromName(elements.second.content)->value.content);
                } else {
                    right = std::stod(elements.second.content);
                }

                list[possibleDiv.second].content = std::to_string(left / right);
                list[possibleDiv.second].type == Lexer::DOUBLE;

                if (possibleDiv.second - 1 >= 0 && possibleDiv.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleDiv.second - 1);
                    list.erase(list.begin() + possibleDiv.second);
                }
                possibleDiv = haveChar(list, "/");
            }

            auto possibleAdd = haveChar(list, "+");
            while (possibleAdd.first) {
                auto elements = getElementsFromIndexChar(list, possibleAdd.second);
                double left = 0;
                double right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    left = std::stod(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stod(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    right = std::stod(data.getVariableFromName(elements.second.content)->value.content);
                } else {
                    right = std::stod(elements.second.content);
                }

                list[possibleAdd.second].content = std::to_string(left + right);
                list[possibleAdd.second].type == Lexer::DOUBLE;

                if (possibleAdd.second - 1 >= 0 && possibleAdd.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleAdd.second - 1);
                    list.erase(list.begin() + possibleAdd.second);
                }

                possibleAdd = haveChar(list, "+");
            }

            auto possibleSub = haveChar(list, "-");
            while (possibleSub.first) {
                auto elements = getElementsFromIndexChar(list, possibleSub.second);
                double left = 0;
                double right = 0;

                if (elements.first.type == Lexer::IDENTIFIANT && data.isVariable(elements.first.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    left = std::stod(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    left = std::stod(elements.first.content);
                }
                if (elements.second.type == Lexer::IDENTIFIANT && data.isVariable(elements.second.content) && (data.getVariableFromName(elements.first.content)->type == Definitions::DOUBLE || data.getVariableFromName(elements.first.content)->type == Definitions::INT)) {
                    right = std::stod(data.getVariableFromName(elements.first.content)->value.content);
                } else {
                    right = std::stod(elements.second.content);
                }

                list[possibleSub.second].content = std::to_string(left - right);
                list[possibleSub.second].type == Lexer::DOUBLE;

                if (possibleSub.second - 1 >= 0 && possibleSub.second + 1 < list.size()) {
                    list.erase(list.begin() + possibleSub.second - 1);
                    list.erase(list.begin() + possibleSub.second);
                }

                possibleSub = haveChar(list, "-");
            }

            value.type = Definitions::DOUBLE;
            value.content = list[list.size()-1].content;
        }
        return value;
    }
}