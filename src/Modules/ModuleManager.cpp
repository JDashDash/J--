#include "ModuleManager.h"

void JDD::Modules::ModulesManager::useStringModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, std::string& content, std::optional<JDD::Definition::Value>& value) {
    auto functionString = ExpectIdentifiant(current);
    if (!functionString.has_value())
        std::cerr << "Specify a function from String module" << std::endl;

    if (functionString.has_value()) {
        if (!ExpectOperator(current, "(").has_value())
            std::cerr << "Forgot to open '()' to give arguments in the String module function" << std::endl;

        if (functionString->content == "concat") {
            auto stringV = ExpectValue(current, data);
            if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                std::cerr << "Request string to use the module and concat function" << std::endl;

            content = JDD::Modules::String::concat(content, stringV->content);
        } else if (functionString->content == "getIndexFromChar") {
            auto stringV = ExpectValue(current, data);
            if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                std::cerr << "Request string to use the module and getIndexFromChar function" << std::endl;

            content += std::to_string(JDD::Modules::String::getIndexFromChar(value->content, stringV->content[0]));
        } else if (functionString->content == "getCharFromIndex") {
            auto stringV = ExpectValue(current, data);
            if (!stringV.has_value() || stringV.value().type != JDD::Definition::INT)
                std::cerr << "Request string to use the module and getCharFromIndex function" << std::endl;

            content += JDD::Modules::String::getCharFromIndex(value->content, std::stoi(stringV->content));
        } else if (functionString->content == "equals") {
            auto stringV = ExpectValue(current, data);
            if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                std::cerr << "Request string to use the module and equals function" << std::endl;

            std::string beforeapply  = std::to_string(JDD::Modules::String::equals(value->content, stringV->content));
            if (beforeapply == "1") {
                content += "true";
            } else {
                beforeapply += "false";
            }
        } else if (functionString->content == "equalsIgnoreCase") {
            auto stringV = ExpectValue(current, data);
            if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                std::cerr << "Request string to use the module and equalsIgnoreCase function" << std::endl;

            std::string beforeapply = std::to_string(JDD::Modules::String::equalsIgnoreCase(value->content, stringV->content));
            if (beforeapply == "1") {
                content += "true";
            } else {
                content += "false";
            }
        } else if (functionString->content == "isEmpty") {
            std::string beforeapply = std::to_string(JDD::Modules::String::isEmpty(value->content));
            if (beforeapply == "1") {
                content += "true";
            } else {
                content += "false";
            }
        } else if (functionString->content == "toLowerCase") {
            content = JDD::Modules::String::toLowerCase(content);
        } else if (functionString->content == "toUpperCase") {
            content = JDD::Modules::String::toUpperCase(content);
        } else if (functionString->content == "replace") {
            auto stringV = ExpectValue(current, data);
            if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                std::cerr << "Request string to use the module and replace function" << std::endl;

            if (!ExpectOperator(current, ",").has_value())
                std::cerr << "Request a second argument to use the module and replace function" << std::endl;

            auto stringV2 = ExpectValue(current, data);
            if (!stringV2.has_value() || stringV2.value().type != JDD::Definition::STRING)
                std::cerr << "Request string to use the module and replace function" << std::endl;

            content += JDD::Modules::String::replace(value->content, stringV->content[0], stringV2->content[0]);
        } else if (functionString->content == "valueOf") {
            auto stringV = ExpectValue(current, data);
            if (!stringV.has_value())
                std::cerr << "Request any value to use the module and replace function" << std::endl;

            content += JDD::Modules::String::valueOf(stringV->content);
        } else {
            std::cerr << "The module String does not understand the function" << std::endl;
        }
        if (!ExpectOperator(current, ")").has_value())
            std::cerr << "Forgot to close the operation(s) with ')'" << std::endl;

    }
}