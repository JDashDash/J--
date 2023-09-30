#include "Parser.h"

#include <utility>
#include <fstream>
#include <vector>

namespace JDD::Parser {
    bool endsWith(std::string_view str, std::string_view suffix) {
        if (str.length() >= suffix.length()) {
            return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
        }
        return false;
    }

    std::string toLower(std::string str) {
        std::string result = std::move(str);
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    void JDDParser::main(const std::vector<JDD::Lexer::Token>& tokenList) {
        Definition::Data data;
        auto current_token = tokenList.begin();

        while (current_token != tokenList.end()) {
            if (!ManagerInstruction(current_token, data, tokenList, true)) {
                std::cerr << "Unknown: " << current_token->content << std::endl;
                ++current_token;
            }
        }
    }

    Definition::Data JDDParser::executeBlocCode(const std::vector<JDD::Lexer::Token>& tokenList, Definition::Data& old_data, bool classicExecution) {
        Definition::Data data;
        for (auto const& var : old_data.Variables) {
            data.pushVariable(var.second);
        }
        auto current_token = tokenList.begin();

        while (current_token != tokenList.end()) {
            if (!ManagerInstruction(current_token, data, tokenList, classicExecution)) {
                std::cerr << "Unknown: " << current_token->content << std::endl;
                ++current_token;
            }
        }

        return data;
    }

    bool JDDParser::ManagerInstruction(std::vector<Lexer::Token>::const_iterator& current, Definition::Data& data, std::vector<Lexer::Token> tokenList, bool classicExecution) {
        auto instruction = ExpectIdentifiant(current);
        if (classicExecution) {
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
            } else if (instruction.has_value() && (instruction->content == "protected" || instruction->content == "private" || instruction->content == "public")) {
                if (instruction->content == "protected")
                    specialVariable_defineFunction(current, data, std::move(tokenList), Definition::FunctionState::FuncProtected);
                else if (instruction->content == "private")
                    specialVariable_defineFunction(current, data, std::move(tokenList), Definition::FunctionState::FuncPrivate);
                else if (instruction->content == "public")
                    specialVariable_defineFunction(current, data, std::move(tokenList), Definition::FunctionState::FuncPublic);
                return true;
            }
                // Special : (a = 10, call a function)
            else if (data.isVariable(instruction->content)) {
                variableManagement(current, data, instruction->content);
                return true;
            } else if (data.isFunction(instruction->content)) {
                functionManagement(current, data, instruction->content);
                return true;
            }
        } else {
            if (instruction.has_value() && (instruction->content == "protected" || instruction->content == "private" || instruction->content == "public")) {
                if (instruction->content == "protected")
                    specialVariable_defineFunction(current, data, std::move(tokenList), Definition::FunctionState::FuncProtected);
                else if (instruction->content == "private")
                    specialVariable_defineFunction(current, data, std::move(tokenList), Definition::FunctionState::FuncPrivate);
                else if (instruction->content == "public")
                    specialVariable_defineFunction(current, data, std::move(tokenList), Definition::FunctionState::FuncPublic);
                return true;
            }
        }
        return false;
    }

    void JDDParser::print(std::vector<Lexer::Token>::const_iterator &current, bool jumpLine, Definition::Data& data) {
        auto openValues = ExpectOperator(current, "(");
        if (!openValues.has_value())
            std::cerr << "Forgot to open with '(' and make sure to close with ')'" << std::endl;

        std::vector<std::string> content;

        while (!ExpectOperator(current, ")").has_value()) {

            auto string = ExpectValue(current, data);
            if (!string.has_value())
                std::cerr << "Specify a value" << std::endl;
            content.push_back(string->content);

            if (ExpectOperator(current, "+").has_value()) {} // Ignore, println("hi" + "cc"); == println("hi" "cc");
        }

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "Forgot to close the instruction with ';'" << std::endl;

        if (jumpLine) { std::cout << "\n"; }
        for (auto const& s : content) {
            std::cout << s;
        }
    }

    void JDDParser::variables(std::vector<Lexer::Token>::const_iterator &current, JDD::Definition::Types type, Definition::Data &data) {
        std::optional<Definition::Types> var_type = type;
        if (type == Definition::FINAL_NotType) {
            var_type = ExpectType(current);
            if (!var_type.has_value())
                std::cerr << "Forgot to give a type to your variable" << std::endl;
        }

        if (var_type == Definition::Types::VOID)
            std::cerr << "A variable can not be void" << std::endl;


        auto var_name = ExpectIdentifiant(current);
        if (!var_name.has_value())
            std::cerr << "Forgot to give a name to your variable" << std::endl;

        if (data.isFunction(var_name->content))
            std::cerr << "A function has already this name" << std::endl;

        if (data.isVariable(var_name->content))
            std::cerr << "A variable has already this name" << std::endl;

        if (var_name->content == "String")
            std::cerr << "Dot not give the name of a module" << std::endl;

        if (!ExpectOperator(current, "=").has_value())
            std::cerr << "Forgot to introduce the value of your variable" << std::endl;

        auto var_value = ExpectValue(current, data);
        if (!var_value.has_value())
            std::cerr << "Forgot to give value to your variable" << std::endl;

        if (ExpectOperator(current, ":").has_value() && data.isVariable(var_value->variableName)) {
            auto action = ExpectIdentifiant(current);
            if (!action.has_value())
                std::cerr << "You have to indicate what action you want to do on your variable" << std::endl;

            if (action->content == "reference") {
                Definition::Variable variable(var_name->content, var_value.value(), var_type.value(), type == JDD::Definition::Types::FINAL_NotType);
                variable.possibleReference_name = var_value->variableName;
                auto referenceVar = data.getVariable(var_value->variableName);
                referenceVar->possibleReference_name = variable.name;

                if (!ExpectOperator(current, ";").has_value())
                    std::cerr << "Forgot to close the instruction with ';'" << std::endl;

                if (var_type != var_value->type)
                    std::cerr << "The variable type is not valid for the value type" << std::endl;

                if (data.isVariable(var_name->content))
                    std::cerr << "The variable already exist" << std::endl;

                data.pushVariable(variable);
                data.pushVariable(referenceVar.value());
            } else
                std::cerr << "The action is not available" << std::endl;
        } else {
            if (!ExpectOperator(current, ";").has_value())
                std::cerr << "Forgot to close the instruction with ';'" << std::endl;

            if (var_type != var_value->type)
                std::cerr << "The variable type is not valid for the value type" << std::endl;

            if (data.isVariable(var_name->content))
                std::cerr << "The variable already exist" << std::endl;

            Definition::Variable variable(var_name->content, var_value.value(), var_type.value(), type == JDD::Definition::Types::FINAL_NotType);
            data.pushVariable(variable);
        }
    }

    void JDDParser::variableManagement(std::vector<Lexer::Token>::const_iterator& current, Definition::Data &data, const std::string& var_name) {
        if (ExpectOperator(current, "=").has_value()) {
            auto value = ExpectValue(current, data);
            if (!value.has_value())
                std::cerr << "Forgot to give the new value to your variable" << std::endl;

            if (!ExpectOperator(current, ";").has_value())
                std::cerr << "forgot to close the instruction with ';'" << std::endl;

            auto var = data.getVariable(var_name);
            if (!var->isFinal) {
                data.updateValueOfVariable(var->name, value->content);
            } else {
                std::cerr << "The variable is declared as final so the action is impossible" << std::endl;
            }

            if (!var->possibleReference_name.empty()) {
                data.updateValueOfVariable(var->possibleReference_name, value->content);
            }
        } else if (ExpectOperator(current, "+").has_value()) {
            if (!ExpectOperator(current, "=").has_value())
                std::cerr << "You need to add '=' to update your variable" << std::endl;

            auto value = ExpectValue(current, data);
            if (!value.has_value())
                std::cerr << "Forgot to give the new value to your variable" << std::endl;

            if (!ExpectOperator(current, ";").has_value())
                std::cerr << "forgot to close the instruction with ';'" << std::endl;

            auto var = data.getVariable(var_name);
            if (!var->isFinal) {
                if (var->type == Definition::INT && value->type == Definition::INT) {
                    int final_value = std::stoi(var->value.content);
                    final_value += std::stoi(value->content);
                    data.updateValueOfVariable(var->name, std::to_string(final_value));
                    if (!var->possibleReference_name.empty()) {
                        data.updateValueOfVariable(var->possibleReference_name, std::to_string(final_value));
                    }
                } else if (var->type == Definition::DOUBLE && value->type == Definition::DOUBLE) {
                    double final_value = std::stod(var->value.content);
                    final_value += std::stod(value->content);
                    data.updateValueOfVariable(var->name, std::to_string(final_value));
                    if (!var->possibleReference_name.empty()) {
                        data.updateValueOfVariable(var->possibleReference_name, std::to_string(final_value));
                    }
                } else if (var->type == Definition::STRING && value->type == Definition::STRING) {
                    std::string final_value = var->value.content;
                    final_value += value->content;
                    data.updateValueOfVariable(var->name, final_value);
                    if (!var->possibleReference_name.empty()) {
                        data.updateValueOfVariable(var->possibleReference_name, final_value);
                    }
                } else {
                    std::cerr << "The value type and variable type is the same" << std::endl;
                }
            } else
                std::cerr << "The variable is declared as final so the action is impossible" << std::endl;
        } else if (ExpectOperator(current, "-").has_value()) {
            if (!ExpectOperator(current, "=").has_value())
                std::cerr << "You need to add '=' to update your variable" << std::endl;

            auto value = ExpectValue(current, data);
            if (!value.has_value())
                std::cerr << "Forgot to give the new value to your variable" << std::endl;

            if (!ExpectOperator(current, ";").has_value())
                std::cerr << "forgot to close the instruction with ';'" << std::endl;

            auto var = data.getVariable(var_name);
            if (!var->isFinal) {
                if (var->type == Definition::INT && value->type == Definition::INT) {
                    int final_value = std::stoi(var->value.content);
                    final_value -= std::stoi(value->content);
                    data.updateValueOfVariable(var->name, std::to_string(final_value));
                    if (!var->possibleReference_name.empty()) {
                        data.updateValueOfVariable(var->possibleReference_name, std::to_string(final_value));
                    }
                } else if (var->type == Definition::DOUBLE && value->type == Definition::DOUBLE) {
                    double final_value = std::stod(var->value.content);
                    final_value -= std::stod(value->content);
                    data.updateValueOfVariable(var->name, std::to_string(final_value));
                    if (!var->possibleReference_name.empty()) {
                        data.updateValueOfVariable(var->possibleReference_name, std::to_string(final_value));
                    }
                } else if (var->type == Definition::STRING && value->type == Definition::STRING) {
                    std::string final_value = var->value.content;
                    size_t foundPos = final_value.find(value->content);
                    if (foundPos != std::string::npos) {
                        final_value.erase(foundPos, value->content.length());
                    }
                    data.updateValueOfVariable(var->name, final_value);
                    if (!var->possibleReference_name.empty()) {
                        data.updateValueOfVariable(var->possibleReference_name, final_value);
                    }
                } else {
                    std::cerr << "The value type and variable type is the same" << std::endl;
                }
            } else
                std::cerr << "The variable is declared as final so the action is impossible" << std::endl;
        } else
            std::cerr << "Forgot to introduce value with '='" << std::endl;
    }

    void JDDParser::specialVariable_defineFunction(std::vector<Lexer::Token>::const_iterator &current, Definition::Data &data, std::vector<Lexer::Token> tokenList, Definition::FunctionState state) {
        bool continueVariable = false;
        bool isProtected = false;
        std::vector<std::string> var_filesName;

        auto type = ExpectType(current);
        if (!type.has_value())
            std::cerr << "Forgot to give a type to your variable or your function" << std::endl;

        std::optional<Definition::Types> var_type = type;
        if (type == Definition::FINAL_NotType) {
            var_type = ExpectType(current);
            if (!var_type.has_value())
                std::cerr << "Forgot to give a type to your variable" << std::endl;
        }

        auto name = ExpectIdentifiant(current);
        if (!name.has_value())
            std::cerr << "Forgot to give a name to your variable or your function" << std::endl;

        if (ExpectOperator(current, "(").has_value()) { // Function
            if (type == Definition::Types::FINAL_NotType)
                std::cerr << "A function can not be declared as final" << std::endl;

            if (data.isFunction(name->content))
                std::cerr << "A function has already this name" << std::endl;

            if (data.isVariable(name->content))
                std::cerr << "A variable has already this name" << std::endl;

            Definition::Function function;
            function.name = name->content;
            function.type = type.value();
            function.state = state;

            while (!ExpectOperator(current, ")").has_value()) {
                auto arg_type = ExpectType(current);
                if (!arg_type.has_value())
                    std::cerr << "Forgot to give a type to your argument" << std::endl;

                auto arg_name = ExpectIdentifiant(current);
                if (!arg_name.has_value())
                    std::cerr << "Forgot to give a name to your argument" << std::endl;

                Definition::Argument arg(arg_name->content, arg_type.value());
                function.pushArgument(arg);

                if (ExpectOperator(current, ")").has_value()) {
                    break;
                } else if (!ExpectOperator(current, ",").has_value())
                    std::cerr << "Awaiting a new argument" << std::endl;
            }

            std::vector<std::string> func_filesName;

            if (state == Definition::FuncProtected) {
                if (!ExpectOperator(current, "[").has_value())
                    std::cerr << "You have to give one/multiple file as string to say which have access to your function" << std::endl;

                while (!ExpectOperator(current, "]").has_value()) {
                    auto fileName = ExpectValue(current, data);
                    if (!fileName.has_value() || fileName->type != Definition::STRING)
                        std::cerr << "You have to give the path file that will can get access to your function" << std::endl;

                    func_filesName.push_back(fileName->content);

                    if (ExpectOperator(current, "]").has_value()) {
                        break;
                    } else if (!ExpectOperator(current, ",").has_value())
                        std::cerr << "Await ',' to give the path file you want to allow access to your function" << std::endl;
                }
            }

            if (!ExpectOperator(current, "{").has_value())
                std::cerr << "Forgot to open the bloc to include your code" << std::endl;

            std::vector<Lexer::Token> innerCodeTokens;
            int nestedBrackets = 1;
            while (current != tokenList.end() && nestedBrackets > 0) {
                if (current->content == "{")
                    nestedBrackets++;
                else if (current->content == "}")
                    nestedBrackets--;
                if (nestedBrackets > 0)
                    innerCodeTokens.push_back(*current);
                current++;
            }

            function.fileAllowAccess = func_filesName;
            function.tokens = innerCodeTokens;

            data.pushFunction(function);
        } else if (ExpectOperator(current, "[").has_value() && state == Definition::FuncProtected) { // Protected Variable
            while (!ExpectOperator(current, "]").has_value()) {
                auto fileName = ExpectValue(current, data);
                if (!fileName.has_value() || fileName->type != Definition::STRING)
                    std::cerr << "You have to give the path file that will can get access to your variable" << std::endl;

                var_filesName.push_back(fileName->content);

                if (ExpectOperator(current, "]").has_value()) {
                    break;
                } else if (!ExpectOperator(current, ",").has_value())
                    std::cerr << "Await ',' to give the path file you want to allow access to your variable" << std::endl;
            }

            if (!ExpectOperator(current, "=").has_value())
                std::cerr << "Forgot to give a operator to specify that you are declaring a variable ('=')" << std::endl;

            continueVariable = true;
            isProtected = true;
        } else if (ExpectOperator(current, "=").has_value() && state != Definition::FuncProtected) { // Public/Private Variable
            continueVariable = true;
            isProtected = false;
        } else {
            std::cerr << "Forgot to give a operator to specify if you are declaring a variable ('=') or a function ('()' for arguments)" << std::endl;
            std::cerr << "Variable Protected have to follow this format: protected TYPE NAME [\"NAME.jdd\"] = VALUE;" << std::endl;
        }

        if (continueVariable) {
            if (data.isFunction(name->content))
                std::cerr << "A function has already this name" << std::endl;

            if (data.isVariable(name->content))
                std::cerr << "A variable has already this name" << std::endl;

            auto var_value = ExpectValue(current, data);
            if (!var_value.has_value())
                std::cerr << "Forgot to give value to your variable" << std::endl;

            if (!ExpectOperator(current, ";").has_value())
                std::cerr << "Forgot to close the instruction with ';'" << std::endl;

            if (var_type != var_value->type)
                std::cerr << "The variable type is not valid for the value type" << std::endl;

            if (data.isVariable(name->content))
                std::cerr << "The variable already exist" << std::endl;

            if (isProtected) {
                Definition::Variable variable(name->content, var_value.value(), var_type.value(), var_type == Definition::FINAL_NotType);
                variable.state = Definition::VarProtected;
                variable.fileAllowAccess = var_filesName;
                data.pushVariable(variable);
            } else {
                Definition::Variable variable(name->content, var_value.value(), var_type.value(), var_type == Definition::FINAL_NotType);
                if (state == Definition::FuncPublic) {
                    variable.state = Definition::VarPublic;
                }
                data.pushVariable(variable);
            }
        }
    }

    void JDDParser::functionManagement(std::vector<Lexer::Token>::const_iterator &current, Definition::Data &data, const std::string& func_name) {
        auto func = data.getFunction(func_name);
        if (!func->arguments.empty()) {
            if (!ExpectOperator(current, "(").has_value())
                std::cerr << "Forgot to open with '(' to specify the value required for arguments in the function you are calling" << std::endl;

            size_t requireArgs = func->arguments.size();
            while (requireArgs > 0) {
                auto name = ExpectIdentifiant(current);
                if (!name.has_value())
                    std::cerr << "Forgot to give the name of the argument" << std::endl;

                if (!ExpectOperator(current, ":").has_value())
                    std::cerr << "Forgot to write ':' to specify the value of the argument next the name" << std::endl;

                auto value = ExpectValue(current, data);
                if (!value.has_value())
                    std::cerr << "Forgot to give the value of the argument called '" + name->content << "'" << std::endl;

                requireArgs -= 1;
                if (requireArgs == 0) {
                    break;
                } else {
                    if (!ExpectOperator(current, ",").has_value())
                        std::cerr << "More arguments are required for the function called '" << func->name << "'" << std::endl;
                }
            }

            if (!ExpectOperator(current, ")").has_value())
                std::cerr << "Forgot to close with ')' when you are calling your function called '" << func->name << "'" << std::endl;
        }

        auto next_data = executeBlocCode(func->tokens, data, true);

        for (auto const& var : next_data.Variables) {
            if (data.isVariable(var.second.name)) {
                data.pushVariable(var.second);
            }
        }

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "Forgot to close the instruction with ';'" << std::endl;
    }

    void JDDParser::import(std::vector<Lexer::Token>::const_iterator &current, Definition::Data &data) {
        auto possibleModule = ExpectIdentifiant(current);
        if (possibleModule.has_value()) {
            if (possibleModule.has_value() && possibleModule->content == "String")
                data.isStringModuleImported = true;
            else if (possibleModule.has_value() && possibleModule->content == "Boolean")
                data.isBooleanModuleImported = true;
            else if (possibleModule.has_value() && possibleModule->content == "Double")
                data.isDoubleModuleImported = true;
            else if (possibleModule.has_value() && possibleModule->content == "Integer")
                data.isIntegerModuleImported = true;
            else
                std::cerr << possibleModule->content << " does not exist in modules added to J--" << std::endl;
        } else {
            auto possibleFile = ExpectValue(current, data);
            if (!possibleFile.has_value() || possibleFile->type != Definition::STRING || !endsWith(toLower(possibleFile->content), toLower(".jdd")))
                std::cerr << "Specify the module (String, Boolean, Double, Integer, or else) or a file with the extension '.jdd'" << std::endl;

            std::ifstream importFile {possibleFile->content};
            if (!importFile) {
                std::cerr << "Unable to open the specified file in import." << std::endl;
            }

            auto asFile = ExpectIdentifiant(current);
            if (!asFile.has_value() || asFile->content != "as") {
                std::cerr << "Specify how you want to import the file " << possibleFile->content << ", what the name you want to give to your current file" << std::endl;
            }

            auto asFileName = ExpectValue(current, data);
            if (!asFileName.has_value() || asFileName->type != Definition::STRING || !endsWith(toLower(asFileName->content), toLower(".jdd")))
                std::cerr << "Specify the module (String, Boolean, Double, Integer, or else) or a file with the extension '.jdd'" << std::endl;


            std::string code((std::istreambuf_iterator<char>(importFile)), (std::istreambuf_iterator<char>()));
            auto tokenList_importFile = JDD::Lexer::Builder::ParserTokens(code);

            auto next_data = executeBlocCode(tokenList_importFile, data, false);
            for (auto var : next_data.Variables) {
                if (var.second.state == Definition::VarPublic) {
                    data.pushVariable(var.second);
                } else if (var.second.state == Definition::VarProtected && var.second.contains_fileAllowAccess(asFileName->content)) {
                    data.pushVariable(var.second);
                }
            }

            for (auto var : next_data.Functions) {
                if (var.second.state == Definition::FuncPublic) {
                    data.pushFunction(var.second);
                } else if (var.second.state == Definition::FuncProtected && var.second.contains_fileAllowAccess(asFileName->content)) {
                    data.pushFunction(var.second);
                }
            }
        }

        if (!ExpectOperator(current, ";").has_value())
            std::cerr << "Forgot to close the instruction with ';'" << std::endl;
    }
}