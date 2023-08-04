#include "ModuleManager.h"

void JDD::Modules::ModulesManager::useStringModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, std::string& content) {
    auto functionString = ExpectIdentifiant(current);
    if (!functionString.has_value())
        std::cerr << "Specify a function from String module" << std::endl;

    if (!ExpectOperator(current, "(").has_value())
        std::cerr << "Open '()' to give arguments to the function" << std::endl;

    if (functionString.has_value() && functionString->content == "concat") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::STRING)
            std::cerr << "'concat' function need string as first parameter" << std::endl;
        content = content + nextString->content;
    } else if (functionString.has_value() && functionString->content == "toLowerCase") {
        content = JDD::Modules::String::toLowerCase(content);
    } else if (functionString.has_value() && functionString->content == "toUpperCase") {
        content = JDD::Modules::String::toUpperCase(content);
    }

    if (!ExpectOperator(current, ")").has_value())
        std::cerr << "Close with ')' to conclude the operation(s)" << std::endl;
}