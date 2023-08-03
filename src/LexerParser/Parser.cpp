#include "Parser.h"

namespace JDD::Parser {
    void JDDParser::main(const std::vector<JDD::Lexer::Token>& tokenList) {
        Definition::Data data;
        auto current_token = tokenList.begin();

        while (current_token != tokenList.end()) {
            if (!ManagerInstruction(current_token, data)) {
                std::cerr << "Unknown: " << current_token->content << std::endl;
                ++current_token;
            }
        }
    }

    bool JDDParser::ManagerInstruction(std::vector<Lexer::Token>::const_iterator& current, Definition::Data& data) {
        auto instruction = ExpectIdentifiant(current);
        if (instruction.has_value() && instruction->content == "print") {
            print(current, false, data);
            return true;
        } else if (instruction.has_value() && instruction->content == "println") {
            print(current, true, data);
            return true;
        } else if (instruction.has_value() && (instruction->content == "int" || instruction->content == "double" ||
                                               instruction->content == "string" || instruction->content == "boolean"
                                               || instruction->content == "final")) {
            if (instruction->content == "int")
                variables(current, JDD::Definition::Types::INT, data);
            else if (instruction->content == "double")
                variables(current, JDD::Definition::Types::DOUBLE, data);
            else if (instruction->content == "string")
                variables(current, JDD::Definition::Types::STRING, data);
            else if (instruction->content == "boolean")
                variables(current, JDD::Definition::Types::BOOLEAN, data);
            else
                variables(current, JDD::Definition::Types::FINAL_NotType, data);
            return true;
        }
        return false;
    }

    void JDDParser::print(std::vector<Lexer::Token>::const_iterator &current, bool jumpLine, Definition::Data& data) {
        auto openValues = ExpectOperator(current, "(");
        if (!openValues.has_value())
            std::cerr << "forgot to open '(' and make sure to close with ')'" << std::endl;

        auto value = ExpectValue(current, data);
        if (!value.has_value())
            std::cerr << "forgot to give value in print" << std::endl;

        if (!ExpectOperator(current, ")").has_value())
            std::cerr << "forgot to close with ')'" << std::endl;

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "forgot to close the instruction with ';'" << std::endl;


        if (jumpLine) { std::cout << "\n"; }
        std::cout << value->content;
    }

    void JDDParser::variables(std::vector<Lexer::Token>::const_iterator &current, JDD::Definition::Types type, Definition::Data& data) {
        std::optional<Definition::Types> var_type;

        if (type == Definition::FINAL_NotType) {
            var_type = ExpectType(current);
            if (!var_type.has_value()) {
                std::cerr << "forgot to give a type to your variable" << std::endl;
            }
        } else {
            var_type = type;
        }

        auto var_name = ExpectIdentifiant(current);
        if (!var_name.has_value())
            std::cerr << "forgot to give a name to your variable" << std::endl;

        if (!ExpectOperator(current, "=").has_value())
            std::cerr << "forgot to introduce the value of your variable" << std::endl;

        auto var_value = ExpectValue(current, data);
        if (!var_value.has_value())
            std::cerr << "forgot to give value to your variable" << std::endl;

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "forgot to close the instruction with ';'" << std::endl;

        if (type != var_value->type)
            std::cerr << "The variable type is not valid for the value type" << std::endl;

        Definition::Variable variable(var_name->content, var_value.value(), var_type.value());
        data.pushVariable(variable);
    }
}