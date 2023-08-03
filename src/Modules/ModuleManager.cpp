#include "ModuleManager.h"

void JDD::Modules::ModulesManager::useStringModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, std::string& content, std::optional<JDD::Definition::Value>& value) {
    if (ExpectOperator(current ,".").has_value()) {
        if (value->type == JDD::Definition::STRING) {
            if (!data.isStringModuleImported)
                std::cerr << "You have to import the module 'String' in your current bloc" << std::endl;

            auto functionString = ExpectIdentifiant(current);
            if (functionString.has_value() && functionString->content == "concat") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                auto stringV = ExpectValue(current, data);
                if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                    std::cerr << "Request string to use the module and concat function" << std::endl;

                content = JDD::Modules::String::concat(value->content, stringV->content);

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            } else if (functionString.has_value() && functionString->content == "getIndexFromChar") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                auto stringV = ExpectValue(current, data);
                if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                    std::cerr << "Request string to use the module and getIndexFromChar function" << std::endl;

                content = std::to_string(JDD::Modules::String::getIndexFromChar(value->content, stringV->content[0]));

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            } else if (functionString.has_value() && functionString->content == "getCharFromIndex") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                auto stringV = ExpectValue(current, data);
                if (!stringV.has_value() || stringV.value().type != JDD::Definition::INT)
                    std::cerr << "Request string to use the module and getCharFromIndex function" << std::endl;

                content = JDD::Modules::String::getCharFromIndex(value->content, std::stoi(stringV->content));

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            } else if (functionString.has_value() && functionString->content == "equals") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                auto stringV = ExpectValue(current, data);
                if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                    std::cerr << "Request string to use the module and equals function" << std::endl;

                content = std::to_string(JDD::Modules::String::equals(value->content, stringV->content));
                if (content == "1") {
                    content = "true";
                } else {
                    content = "false";
                }

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            } else if (functionString.has_value() && functionString->content == "equalsIgnoreCase") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                auto stringV = ExpectValue(current, data);
                if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                    std::cerr << "Request string to use the module and equalsIgnoreCase function" << std::endl;

                content = std::to_string(JDD::Modules::String::equalsIgnoreCase(value->content, stringV->content));
                if (content == "1") {
                    content = "true";
                } else {
                    content = "false";
                }

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            } else if (functionString.has_value() && functionString->content == "isEmpty") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                content = std::to_string(JDD::Modules::String::isEmpty(value->content));
                if (content == "1") {
                    content = "true";
                } else {
                    content = "false";
                }

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            } else if (functionString.has_value() && functionString->content == "toLowerCase") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                content = JDD::Modules::String::toLowerCase(value->content);

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            } else if (functionString.has_value() && functionString->content == "toUpperCase") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                content = JDD::Modules::String::toUpperCase(value->content);

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            }  else if (functionString.has_value() && functionString->content == "replace") {
                if (!ExpectOperator(current, "(").has_value())
                    std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

                auto stringV = ExpectValue(current, data);
                if (!stringV.has_value() || stringV.value().type != JDD::Definition::STRING)
                    std::cerr << "Request string to use the module and replace function" << std::endl;

                if (!ExpectOperator(current, ",").has_value())
                    std::cerr << "Request a second argument to use the module and replace function" << std::endl;

                auto stringV2 = ExpectValue(current, data);
                if (!stringV2.has_value() || stringV2.value().type != JDD::Definition::STRING)
                    std::cerr << "Request string to use the module and replace function" << std::endl;

                content = JDD::Modules::String::replace(value->content, stringV->content[0], stringV2->content[0]);

                if (!ExpectOperator(current, ")").has_value())
                    std::cerr << "Forgot to close with ')'" << std::endl;
            }
            else
                std::cerr << "This function is the String module is not available" << std::endl;
        }
    }
}