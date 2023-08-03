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
        } else if (instruction.has_value() && instruction->content == "import") {
            import(current, data);
            return true;
        }
        // Special : (a = 10, call a function)
        else if (data.isVariable(instruction->content)) {
            updateVariableValue(current, data, instruction->content);
            return true;
        }
        return false;
    }

    void JDDParser::print(std::vector<Lexer::Token>::const_iterator &current, bool jumpLine, Definition::Data& data) {
        auto openValues = ExpectOperator(current, "(");
        if (!openValues.has_value())
            std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

        auto value = ExpectValue(current, data);
        if (!value.has_value()) {
            std::cerr << "Specify a value" << std::endl;
        }

        std::string content = value->content;
        if (value.has_value() && ExpectOperator(current, ".").has_value() && value->type == Definition::STRING && data.isStringModuleImported) {
            JDD::Modules::ModulesManager::useStringModule(current, data, content, value);
        }

        if (!ExpectOperator(current, ")").has_value())
            std::cerr << "Forgot to close with ')'" << std::endl;

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "Forgot to close the instruction with ';'" << std::endl;

        if (jumpLine) { std::cout << "\n"; }
        std::cout << content;
    }

    void JDDParser::variables(std::vector<Lexer::Token>::const_iterator &current, JDD::Definition::Types type, Definition::Data& data) {
        std::optional<Definition::Types> var_type = type;
        if (type == Definition::FINAL_NotType) {
            var_type = ExpectType(current);
            if (!var_type.has_value())
                std::cerr << "Forgot to give a type to your variable" << std::endl;
        }

        auto var_name = ExpectIdentifiant(current);
        if (!var_name.has_value())
            std::cerr << "Forgot to give a name to your variable" << std::endl;

        if (var_name->content == "String")
            std::cerr << "Dot not give the name of a module" << std::endl;

        if (!ExpectOperator(current, "=").has_value())
            std::cerr << "Forgot to introduce the value of your variable" << std::endl;

        auto var_value = ExpectValue(current, data);
        if (!var_value.has_value())
            std::cerr << "Forgot to give value to your variable" << std::endl;

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "Forgot to close the instruction with ';'" << std::endl;

        if (var_type != var_value->type)
            std::cerr << "The variable type is not valid for the value type" << std::endl;

        if (data.isVariable(var_name->content))
            std::cerr << "The variable already exist" << std::endl;

        Definition::Variable variable(var_name->content, var_value.value(), var_type.value(), type == Definition::FINAL_NotType);
        data.pushVariable(variable);
    }

    void JDDParser::updateVariableValue(std::vector<Lexer::Token>::const_iterator& current, Definition::Data &data, const std::string& var_name) {
        if (!ExpectOperator(current, "=").has_value())
            std::cerr << "Forgot to introduce value with '='" << std::endl;

        auto value = ExpectValue(current, data);
        if (!value.has_value())
            std::cerr << "Forgot to give the new value to your variable" << std::endl;

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "forgot to close the instruction with ';'" << std::endl;

        auto var = data.getVariable(var_name);
        if (!var->isFinal)
            data.updateValueOfVariable(var->name, value->content);
        else
            std::cerr << "The variable is declared as final so the action is impossible" << std::endl;
    }

    void JDDParser::import(std::vector<Lexer::Token>::const_iterator &current, Definition::Data& data) {
        auto possibleModule = ExpectIdentifiant(current);
        if (!possibleModule.has_value())
            std::cerr << "Specify a module" << std::endl;

        if (possibleModule.has_value() && possibleModule->content == "String") {
            data.isStringModuleImported = true;
        }

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "Forgot to close the instruction with ';'" << std::endl;
    }
}