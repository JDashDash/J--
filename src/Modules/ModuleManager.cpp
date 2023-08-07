#include "ModuleManager.h"
#include "Boolean/Boolean.h"

void JDD::Modules::ModulesManager::useStringModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, Definition::Value& value) {
    auto functionString = ExpectIdentifiant(current);
    if (!functionString.has_value())
        std::cerr << "Specify a function from String module" << std::endl;

    if (!ExpectOperator(current, "(").has_value())
        std::cerr << "Open '()' to give arguments to the function" << std::endl;

    if (functionString.has_value() && functionString->content == "concat") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value())
            std::cerr << "'concat' function need a value or can give a variable as first parameter" << std::endl;
        value.content = JDD::Modules::String::concat(value.content, nextString->content);
    } else if (functionString.has_value() && functionString->content == "getIndexFromChar") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::STRING)
            std::cerr << "'getIndexFromChar' function need a string value as first parameter" << std::endl;
        value.content = std::to_string(JDD::Modules::String::getIndexFromChar(value.content, nextString->content[0]));
        value.type = Definition::Types::INT;
    } else if (functionString.has_value() && functionString->content == "getCharFromIndex") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::INT)
            std::cerr << "'getCharFromIndex' function need a int value as first parameter" << std::endl;
        value.content = std::to_string(JDD::Modules::String::getCharFromIndex(value.content, std::stoi(nextString->content)));
    }  else if (functionString.has_value() && functionString->content == "equals") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::STRING)
            std::cerr << "'equals' function need string as first parameter" << std::endl;
        value.content = std::to_string(JDD::Modules::String::equals(value.content, nextString->content));
        if (value.content == "1") value.content = "true";
        else  value.content = "false";
        
        value.type = Definition::Types::BOOLEAN;
    } else if (functionString.has_value() && functionString->content == "equalsIgnoreCase") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::STRING)
            std::cerr << "'equalsIgnoreCase' function need string as first parameter" << std::endl;
        value.content = std::to_string(JDD::Modules::String::equalsIgnoreCase(value.content, nextString->content));
        if (value.content == "1") value.content = "true"; 
        else  value.content = "false"; 

        value.type = Definition::Types::BOOLEAN;
    } else if (functionString.has_value() && functionString->content == "isEmpty") {
        value.content = std::to_string(JDD::Modules::String::isEmpty(value.content));
        if (value.content == "1") value.content = "true";
        else value.content = "false";
        
        value.type = Definition::Types::BOOLEAN;
    } else if (functionString.has_value() && functionString->content == "toLowerCase") {
        value.content = JDD::Modules::String::toLowerCase(value.content);
    } else if (functionString.has_value() && functionString->content == "toUpperCase") {
        value.content = JDD::Modules::String::toUpperCase(value.content);
    } else if (functionString.has_value() && functionString->content == "replace") {
        auto firstChar = ExpectValue(current, data);

        if (!firstChar.has_value() || firstChar->type != Definition::STRING)
            std::cerr << "'replace' function need string value as first parameter" << std::endl;

        if (!ExpectOperator(current, ",").has_value())
            std::cerr << "Need ',' to give the next argument" << std::endl;

        auto secondChar = ExpectValue(current, data);
        if (!secondChar.has_value() || secondChar->type != Definition::STRING)
            std::cerr << "'replace' function need string value as second parameter" << std::endl;

        value.content = JDD::Modules::String::replace(value.content, firstChar->content[0], secondChar->content[0]);
    } else if (functionString.has_value() && functionString->content == "valueOf") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value())
            std::cerr << "'valueOf' function need a value or can give a variable as first parameter" << std::endl;
        value.content = nextString->content;
    } else
        std::cerr << "This function from String is not available (" << functionString->content << " does not exist)" << std::endl;


    if (!ExpectOperator(current, ")").has_value())
        std::cerr << "Close with ')' to conclude the operation(s)" << std::endl;
}

void JDD::Modules::ModulesManager::useBooleanModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, Definition::Value& value) {
    auto functionString = ExpectIdentifiant(current);
    if (!functionString.has_value())
        std::cerr << "Specify a function from String module" << std::endl;

    if (!ExpectOperator(current, "(").has_value())
        std::cerr << "Open '()' to give arguments to the function" << std::endl;

    if (functionString.has_value() && functionString->content == "toInt") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::BOOLEAN)
            std::cerr << "'toInt' function need a boolean value as first parameter" << std::endl;
        if (nextString->content == "true") value.content = std::to_string(JDD::Modules::Boolean::toInt(true));
        else value.content = std::to_string(JDD::Modules::Boolean::toInt(false));
        
        value.type = Definition::Types::INT;
    } else if (functionString.has_value() && functionString->content == "fromInt") {
        auto nextString = ExpectValue(current, data);
        if (!nextString.has_value() || nextString->type != Definition::INT)
            std::cerr << "'fromInt' function need a boolean value as first parameter" << std::endl;
        if (nextString->content == "1") value.content = std::to_string(JDD::Modules::Boolean::fromInt(1));
        else value.content = std::to_string(JDD::Modules::Boolean::fromInt(0));
        if (value.content == "1") {
            value.content = "true";
        } else {
            value.content = "false";
        }
    } else if (functionString.has_value() && functionString->content == "opposite") {
        if (value.content == "1" || value.content == "true") value.content = std::to_string(JDD::Modules::Boolean::opposite(false));
        else value.content = std::to_string(JDD::Modules::Boolean::opposite(true));
        if (value.content == "0") {
            value.content = "true";
        } else {
            value.content = "false";
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

        if (firstValue->type == Definition::INT)
            value.content = std::to_string(JDD::Modules::Boolean::compare(std::stoi(firstValue->content) ,std::stoi(secondValue->content), condOp.value()));
        else
            value.content = std::to_string(JDD::Modules::Boolean::compare(std::stod(firstValue->content) ,std::stod(secondValue->content), condOp.value()));
        
        if (value.content == "1") value.content = "true";
        else value.content = "false";
        
    } else
        std::cerr << "This function from String is not available (" << functionString->content << " does not exist)" << std::endl;


    if (!ExpectOperator(current, ")").has_value())
        std::cerr << "Close with ')' to conclude the operation(s)" << std::endl;
}