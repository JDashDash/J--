#include "ModuleManager.h"
#include "Boolean/Boolean.h"

void JDD::Modules::ModulesManager::useStringModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, std::string& content) {
    auto functionString = ExpectIdentifiant(current);
    if (!functionString.has_value())
        std::cerr << "Specify a function from String module" << std::endl;

    if (!ExpectOperator(current, "(").has_value())
        std::cerr << "Open '()' to give arguments to the function" << std::endl;

    if (functionString.has_value() && functionString->content == "concat") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value())
            std::cerr << "'concat' function need a value or can give a variable as first parameter" << std::endl;
        content = JDD::Modules::String::concat(content, nextString->content);
    } else if (functionString.has_value() && functionString->content == "getIndexFromChar") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::STRING)
            std::cerr << "'getIndexFromChar' function need a string value as first parameter" << std::endl;
        content = std::to_string(JDD::Modules::String::getIndexFromChar(content, nextString->content[0]));
    } else if (functionString.has_value() && functionString->content == "getCharFromIndex") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::INT)
            std::cerr << "'getCharFromIndex' function need a int value as first parameter" << std::endl;
        content = std::to_string(JDD::Modules::String::getCharFromIndex(content, std::stoi(nextString->content)));
    }  else if (functionString.has_value() && functionString->content == "equals") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::STRING)
            std::cerr << "'equals' function need string as first parameter" << std::endl;
        content = std::to_string(JDD::Modules::String::equals(content, nextString->content));
    } else if (functionString.has_value() && functionString->content == "equalsIgnoreCase") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::STRING)
            std::cerr << "'equalsIgnoreCase' function need string as first parameter" << std::endl;
        content = std::to_string(JDD::Modules::String::equalsIgnoreCase(content, nextString->content));
        if (content == "1") {
            content = "true";
        } else {
            content = "false";
        }
    } else if (functionString.has_value() && functionString->content == "isEmpty") {
        content = std::to_string(JDD::Modules::String::isEmpty(content));
        if (content == "1") {
            content = "true";
        } else {
            content = "false";
        }
    } else if (functionString.has_value() && functionString->content == "toLowerCase") {
        content = JDD::Modules::String::toLowerCase(content);
    } else if (functionString.has_value() && functionString->content == "toUpperCase") {
        content = JDD::Modules::String::toUpperCase(content);
    } else if (functionString.has_value() && functionString->content == "replace") {
        auto firstChar = ExpectValue(current, data);

        if (!firstChar.has_value() || firstChar->type != Definition::STRING)
            std::cerr << "'replace' function need string value as first parameter" << std::endl;

        if (!ExpectOperator(current, ",").has_value())
            std::cerr << "Need ',' to give the next argument" << std::endl;

        auto secondChar = ExpectValue(current, data);
        if (!secondChar.has_value() || secondChar->type != Definition::STRING)
            std::cerr << "'replace' function need string value as second parameter" << std::endl;

        content = JDD::Modules::String::replace(content, firstChar->content[0], secondChar->content[0]);
    } else if (functionString.has_value() && functionString->content == "valueOf") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value())
            std::cerr << "'valueOf' function need a value or can give a variable as first parameter" << std::endl;
        content = nextString->content;
    } else
        std::cerr << "This function from String is not available (" << functionString->content << " does not exist)" << std::endl;


    if (!ExpectOperator(current, ")").has_value())
        std::cerr << "Close with ')' to conclude the operation(s)" << std::endl;
}

void JDD::Modules::ModulesManager::useBooleanModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, std::string& content) {
    auto functionString = ExpectIdentifiant(current);
    if (!functionString.has_value())
        std::cerr << "Specify a function from String module" << std::endl;

    if (!ExpectOperator(current, "(").has_value())
        std::cerr << "Open '()' to give arguments to the function" << std::endl;

    if (functionString.has_value() && functionString->content == "toInt") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::BOOLEAN)
            std::cerr << "'toInt' function need a boolean value as first parameter" << std::endl;
        if (nextString->content == "true") content = std::to_string(JDD::Modules::Boolean::toInt(true));
        else content = std::to_string(JDD::Modules::Boolean::toInt(false));
    } else if (functionString.has_value() && functionString->content == "fromInt") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::INT)
            std::cerr << "'fromInt' function need a boolean value as first parameter" << std::endl;
        if (nextString->content == "1") content = std::to_string(JDD::Modules::Boolean::fromInt(1));
        else content = std::to_string(JDD::Modules::Boolean::fromInt(0));
        if (content == "1") {
            content = "true";
        } else {
            content = "false";
        }
    } else if (functionString.has_value() && functionString->content == "opposite") {
        if (content == "1" || content == "true") content = std::to_string(JDD::Modules::Boolean::opposite(false));
        else content = std::to_string(JDD::Modules::Boolean::opposite(true));
        if (content == "0") {
            content = "true";
        } else {
            content = "false";
        }
    } else if (functionString.has_value() && functionString->content == "compare") {
        auto firstValue = ExpectValue(current, data);
        if (!firstValue.has_value() || (firstValue->type != Definition::DOUBLE && firstValue->type != Definition::INT))
            std::cerr << "'compare' function need a int or a double value as first parameter" << std::endl;

        auto condOp = ExpectConditionalOperator(current);
        if (!condOp.has_value())
            std::cerr << "'compare' function need a conditional operator (!=, >=, <=, ==, < or >) to compare as second parameter" << std::endl;

        auto secondValue = ExpectValue(current, data);
        if (!secondValue.has_value() || (secondValue->type != Definition::DOUBLE && secondValue->type != Definition::INT))
            std::cerr << "'compare' function need a int or a double value as third parameter" << std::endl;

        if (firstValue->type == Definition::INT) {
            content = std::to_string(JDD::Modules::Boolean::compare(std::stoi(firstValue->content) ,std::stoi(secondValue->content), condOp.value()));
        } else {
            content = std::to_string(JDD::Modules::Boolean::compare(std::stod(firstValue->content) ,std::stod(secondValue->content), condOp.value()));
        }
        if (content == "1") {
            content = "true";
        } else {
            content = "false";
        }
    } else
        std::cerr << "This function from String is not available (" << functionString->content << " does not exist)" << std::endl;


    if (!ExpectOperator(current, ")").has_value())
        std::cerr << "Close with ')' to conclude the operation(s)" << std::endl;
}