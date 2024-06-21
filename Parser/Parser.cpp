#include "Parser.h"

namespace JDD::Parser {
    bool containsComma(const std::vector<JDD::Lexer::Token>& vec) {
        return std::ranges::any_of(vec, [](const JDD::Lexer::Token& token) {
            return token.content == ",";
        });
    }

    std::vector<std::vector<JDD::Lexer::Token>> splitByComma(const std::vector<JDD::Lexer::Token>& vec) {
        std::vector<std::vector<JDD::Lexer::Token>> result;
        std::vector<JDD::Lexer::Token> currentSegment;

        for (const auto& token : vec) {
            if (token.content == ",") {
                if (!currentSegment.empty()) {
                    result.push_back(currentSegment);
                    currentSegment.clear();
                }
            } else {
                currentSegment.push_back(token);
            }
        }
        if (!currentSegment.empty()) {
            result.push_back(currentSegment);
        }

        return result;
    }



    void run(std::vector<JDD::Lexer::Token>& tokensList)  {
        Data data;
        auto current = tokensList.begin();

        while (current != tokensList.end()) {
            if (!instructionsManagement(data, tokensList, current)) {
                std::cerr  << "{UNKNOWN} -> " << *current << std::endl;
                ++current;
            }
        }
    }

    void runCodeBlock(std::vector<JDD::Lexer::Token>& tokenBlock, Data& globalData) {
        auto current = tokenBlock.begin();
        while (current != tokenBlock.end()) {
            if (!instructionsManagement(globalData, tokenBlock, current)) {
                std::cerr  << "{UNKNOWN} -> " << *current << std::endl;
                ++current;
            }
        }
    }

    void print(Data &data, std::vector<JDD::Lexer::Token> &vector, std::vector<Token>::iterator &iterator);

    bool instructionsManagement(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current) {
        auto instruction = ExpectIdentifiant(current);

        if (instruction.has_value() && instruction->content == "int") {
            basicVariableDeclaration(data, tokensList, current, JDD::Definitions::Types::INT);
            return true;
        } else if (instruction.has_value() && instruction->content == "double") {
            basicVariableDeclaration(data, tokensList, current, JDD::Definitions::Types::DOUBLE);
            return true;
        } else if (instruction.has_value() && instruction->content == "string") {
            basicVariableDeclaration(data, tokensList, current, JDD::Definitions::Types::STRING);
            return true;
        } else if (instruction.has_value() && instruction->content == "boolean") {
            basicVariableDeclaration(data, tokensList, current, JDD::Definitions::Types::BOOLEAN);
            return true;
        } else if (instruction.has_value() && instruction->content == "final") {
            basicVariableDeclaration(data, tokensList, current, JDD::Definitions::Types::FINAL_NotType);
            return true;
        } else if (instruction.has_value() && instruction->content == "delete") {
            deleteVariableOrFunction(data, tokensList, current);
            return true;
        } else if (instruction.has_value() && instruction->content == "assert_eq") {
            assert_eq(data, tokensList, current);
            return true;
        } else if (instruction.has_value() && instruction->content == "public") {
            functionDeclaration(data, tokensList, current, JDD::Definitions::FunctionState::FuncPublic);
            return true;
        } else if (instruction.has_value() && instruction->content == "private") {
            functionDeclaration(data, tokensList, current, JDD::Definitions::FunctionState::FuncPrivate);
            return true;
        } else if (instruction.has_value() && instruction->content == "protected") {
            functionDeclaration(data, tokensList, current, JDD::Definitions::FunctionState::FuncProtected);
            return true;
        } else if (instruction.has_value() && instruction->content == "print") {
            print(data, tokensList, current, false);
            return true;
        } else if (instruction.has_value() && instruction->content == "println") {
            print(data, tokensList, current, true);
            return true;
        } else if (instruction.has_value()) {
            managementVariablesFunctionsCallModification(data, tokensList, current, instruction.value());
            return true;
        }
        return false;
    }

    void print(Data &data, std::vector<JDD::Lexer::Token> &tokensList, std::vector<Token>::iterator &current, bool jumpLine) {
        if (!ExpectOperator(current, "(").has_value()) {
            std::cerr << "[PRINT] To give input open '()', line " << current->line << std::endl;
            exit(22);
        }



        std::vector<Token> contentPrint;

        while (!ExpectOperator(current, ")").has_value()) {
            contentPrint.push_back(*current);
            ++current;
        }

        std::string finalResultPrint;

        if (containsComma(contentPrint)) {
            auto valuesInPrint = splitByComma(contentPrint);
            for (auto e : valuesInPrint) {
                JDD::Definitions::Types awaitingType = JDD::Definitions::VOID;
                auto v = ReturnFinalValueFromListToken(data, current, e, awaitingType);
                finalResultPrint += v.content;
            }
        } else {
            JDD::Definitions::Types awaitingType = JDD::Definitions::VOID;
            auto v = ReturnFinalValueFromListToken(data, current, contentPrint, awaitingType);
            finalResultPrint += v.content;
        }

        if (!ExpectOperator(current, ";").has_value()) {
            std::cerr << "[PRINT] Need ';' operator to conclude the instruction, line " << current->line << std::endl;
            exit(6);
        }

        if (jumpLine) {
            std::cout << "\n" << finalResultPrint;
        } else {
            std::cout << finalResultPrint;
        }
    }

    void basicVariableDeclaration(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, JDD::Definitions::Types type) {
        std::optional<Definitions::Types> var_type = type;
        bool isFinal = false;
        if (type == Definitions::FINAL_NotType) {
            var_type = ExpectType(current);
            isFinal = true;
            if (!var_type.has_value()) {
                std::cerr << "[VARIABLE DECLARATION] Need a type for your variable" << std::endl;
                exit(1);
            }
        }

        if (var_type == Definitions::Types::VOID) {
            std::cerr << "[VARIABLE DECLARATION] A variable can not be void" << std::endl;
            exit(2);
        }

        auto name = ExpectIdentifiant(current);
        if (!name.has_value()) {
            std::cerr << "[VARIABLE DECLARATION] Need the name of your variable, line " << current->line << std::endl;
            exit(3);
        }

        if (!ExpectOperator(current, "=").has_value()) {
            std::cerr << "[VARIABLE DECLARATION] Need '=' operator to give your value, line " << current->line << std::endl;
            exit(4);
        }

        std::vector<Token> contentValueVariable;

        while (!ExpectOperator(current, ";").has_value()) {
            contentValueVariable.push_back(*current);
            ++current;
        }

        if (data.isVariable(name->content)) {
            std::cerr << "[VARIABLE DECLARATION] This variable has been already declared, line " << current->line << std::endl;
            exit(7);
        }

        Variable var;
        var.name = name->content;
        var.type = type;
        var.isFinal = isFinal;
        var.value = ReturnFinalValueFromListToken(data, current, contentValueVariable, var.type);
        data.addVariableToData(var);
    }

    void managementVariablesFunctionsCallModification(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, Token& id) {
        if (!data.isVariable(id.content) && !data.isFunction(id.content)) {
            std::cerr << "[OPERATIONS: Variables - Functions] The variable/function does not exist, line " << current->line << std::endl;
            exit(8);
        }

        auto var = data.getVariableFromName(id.content);
        if (var.has_value()) {
            if (var->isFinal) {
                std::cerr << "[OPERATIONS: Variables] Operations are forbidden on final (const) variables, line " << current->line << std::endl;
                exit(16);
            }

            std::string operatorVar = "=";

            if (ExpectOperator(current, "+")) {
                if (!ExpectOperator(current, "=").has_value()) {
                    std::cerr << "[OPERATIONS: Variables] Need '=' operator to increment variable's value with the value given, line " << current->line << std::endl;
                    exit(9);
                }
                operatorVar = "+=";
            } else if (ExpectOperator(current, "-")) {
                if (!ExpectOperator(current, "=").has_value()) {
                    std::cerr << "[OPERATIONS: Variables] Need '=' operator to decrement variable's value with the value given, line " << current->line << std::endl;
                    exit(10);
                }
                operatorVar = "-=";
            } else if (!ExpectOperator(current, "=").has_value()) {
                std::cerr << "[OPERATIONS: Variables] Need '=' operator to give a new value to the variable, line " << current->line << std::endl;
                exit(11);
            }

            auto value = ExpectValue(current);
            if (value->type != var->type) {
                std::cerr << "[OPERATIONS: Variables] The variable and value type are not equal, line " << current->line << std::endl;
                exit(12);
            }

            if (var->type != Definitions::INT && var->type != Definitions::DOUBLE && var->type != Definitions::STRING) {
                std::cerr << "[OPERATIONS: Variables] The variable needs to be an INT, DOUBLE or a STRING, line " << current->line << std::endl;
                exit(13);
            }

            if (var->isFinal) {
                std::cerr << "[OPERATIONS: Variables] The variable is final (const) so not updatable, line " << current->line << std::endl;
                exit(14);
            }

            if (operatorVar == "=") {
                data.updateVariableValueFromData(id.content, value->content);
            } else if (operatorVar == "+=" && var->type == Definitions::STRING) {
                data.updateVariableValueFromData(id.content,
                    var->value.content + value->content
                );
            } else if (operatorVar == "-=" && var->type == Definitions::STRING) {
                data.updateVariableValueFromData(id.content,
                    var->value.content.erase(var->value.content.find(value->content))
                );
            } else if (operatorVar == "+=" && var->type == Definitions::INT) {
                int left = std::stoi(var->value.content);
                int right = std::stoi(value->content);
                data.updateVariableValueFromData(id.content, std::to_string(left + right));
            } else if (operatorVar == "-=" && var->type == Definitions::INT) {
                int left = std::stoi(var->value.content);
                int right = std::stoi(value->content);
                data.updateVariableValueFromData(id.content, std::to_string(left - right));
            } else if (operatorVar == "+=" && var->type == Definitions::DOUBLE) {
                double left = std::stod(var->value.content);
                double right = std::stod(value->content);
                data.updateVariableValueFromData(id.content, std::to_string(left + right));
            } else if (operatorVar == "-=" && var->type == Definitions::DOUBLE) {
                double left = std::stod(var->value.content);
                double right = std::stod(value->content);
                data.updateVariableValueFromData(id.content, std::to_string(left - right));
            }
        } else if (data.isFunction(id.content)) {
            auto func = data.getFunctionFromName(id.content);
            runCodeBlock(func->content_tokens, data);
        }

        if (!ExpectOperator(current, ";")) {
            std::cerr << "[OPERATIONS: Variables] Need ';' operator to conclude the instruction, line " << current->line << std::endl;
            exit(6);
        }
    }

    void deleteVariableOrFunction(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current) {
        auto id = ExpectIdentifiant(current);
        if (!id.has_value()) {
            std::cerr << "[DELETE: Variables - Functions] Need the name of your variable/function, line " << current->line << std::endl;
            exit(3);
        }

        if (!data.getVariableFromName(id->content).has_value()) {
            std::cerr << "[DELETE: Variables - Functions] The variable/function does not exist, line " << current->line << std::endl;
            exit(7);
        }

        auto var = data.getVariableFromName(id->content);
        if (var.has_value()) {
            data.removeVariableFromName(id->content);
            if (!ExpectOperator(current, ";")) {
                std::cerr << "[DELETE: Variables] Need ';' operator to conclude the instruction, line " << current->line << std::endl;
                exit(6);
            }
        }
    }

    void assert_eq(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current) {
        auto first_var_name = ExpectIdentifiant(current);
        if (!first_var_name.has_value()) {
            std::cerr << "[ASSERT_EQ] Need the name of your variable, line " << current->line << std::endl;
            exit(3);
        }
        if (!data.isVariable(first_var_name->content)) {
            std::cerr << "[ASSERT_EQ] The variable does not exist, line " << current->line << std::endl;
            exit(8);
        }
        if (!ExpectOperator(current, ",").has_value()) {
            std::cerr << "[ASSERT_EQ] Need ',' operator, line " << current->line << std::endl;
            exit(15);
        }

        auto second_var_name = ExpectIdentifiant(current);
        if (!second_var_name.has_value()) {
            std::cerr << "[ASSERT_EQ] Need the name of your variable, line " << current->line << std::endl;
            exit(3);
        }
        if (!data.isVariable(second_var_name->content)) {
            std::cerr << "[ASSERT_EQ] The variable does not exist, line " << current->line << std::endl;
            exit(8);
        }

        auto var1 = data.getVariableFromName(first_var_name->content);
        auto var2 = data.getVariableFromName(second_var_name->content);
        std::string message;

        if (ExpectOperator(current, ",").has_value()) {
            auto possibleMessage = ExpectValue(current);
            if (possibleMessage.has_value() && possibleMessage->type == Definitions::STRING) {
                message = possibleMessage->content;
            }
        }

        if (var1->value.content != var2->value.content) {
            if (!message.empty()) {
                std::cerr << "Assert Error: '" << message << "', line " << current->line << std::endl;
            } else {
                std::cerr << "Assert Error, line " << current->line << std::endl;
            }
            exit(0);
        }

        if (!ExpectOperator(current, ";").has_value()) {
            std::cerr << "[ASSERT_EQ] Need ';' operator to conclude the instruction, line " << current->line << std::endl;
            exit(6);
        }
    }

    void functionDeclaration(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, JDD::Definitions::FunctionState state) {
        Definitions::Types type;

        auto type_str = ExpectIdentifiant(current);
        if (type_str.has_value() && type_str->content == "void") {
            type = Definitions::VOID;
        } else if (type_str.has_value() && type_str->content == "int") {
            type = Definitions::INT;
        } else if (type_str.has_value() && type_str->content == "double") {
            type = Definitions::DOUBLE;
        } else if (type_str.has_value() && type_str->content == "string") {
            type = Definitions::STRING;
        } else if (type_str.has_value() && type_str->content == "boolean") {
            type = Definitions::BOOLEAN;
        } else {
            std::cerr << "[FUNCTION DECLARATION] Need a type for your function" << std::endl;
            exit(1);
        }

        auto name_str = ExpectIdentifiant(current);
        if (!name_str.has_value()) {
            std::cerr << "[FUNCTION DECLARATION] Need the name of your function, line " << current->line << std::endl;
            exit(3);
        }

        Function function;
        function.name = name_str->content;
        function.returnVariable.type = type;
        function.state = state;

        if (ExpectOperator(current, "(").has_value()) {
            while (!ExpectOperator(current, ")").has_value()) {
                auto t = ExpectIdentifiant(current);
                if (!t.has_value()) {
                    std::cerr << "[FUNCTION DECLARATION] Give the type of you argument, line " << current->line << std::endl;
                    exit(18);
                }

                auto n = ExpectIdentifiant(current);
                if (!n.has_value()) {
                    std::cerr << "[FUNCTION DECLARATION] Give the name of you argument, line " << current->line << std::endl;
                    exit(19);
                }

                Definitions::Types type_arg;
                std::string name_arg = n->content;

                if (t.has_value() && t->content == "void") {
                    type_arg = Definitions::VOID;
                } else if (t.has_value() && t->content == "int") {
                    type_arg = Definitions::INT;
                } else if (t.has_value() && t->content == "double") {
                    type_arg = Definitions::DOUBLE;
                } else if (t.has_value() && t->content == "string") {
                    type_arg = Definitions::STRING;
                } else if (t.has_value() && t->content == "boolean") {
                    type_arg = Definitions::BOOLEAN;
                } else {
                    std::cerr << "[FUNCTION DECLARATION] Need a type for your function" << std::endl;
                    exit(1);
                }

                Argument arg(type_arg, name_arg);
                function.pushArgument(arg);

                if (!ExpectOperator(current, ",").has_value() && ExpectOperator(current, ")").has_value()) {
                    break;
                } else if (ExpectOperator(current, ",").has_value() && ExpectOperator(current, ")").has_value()) {
                    std::cerr << "[FUNCTION DECLARATION] Waiting another argument, remove ',' if you don't want to add more arguments into your function, line " << current->line << std::endl;
                    exit(20);
                }
            }
        } else {
            std::cerr << "[FUNCTION DECLARATION] To give arguments open with '(argType argName, ...)', if you don't want to define arguments just add '()', line " << current->line << std::endl;
            exit(17);
        }

        if (ExpectOperator(current, "{")) {
            int closeRequired = 1; // Number required : }
            while (closeRequired > 0) {
                if (ExpectOperator(current, "}").has_value()) {
                    closeRequired -= 1;
                } else if (ExpectOperator(current, "{").has_value()) {
                    closeRequired += 1;
                }
                if (closeRequired > 0 && tokensList.end() != current) {
                    function.content_tokens.push_back(*current);
                    ++current;
                }
            }
        } else {
            std::cerr << "[FUNCTION DECLARATION] In a function you give a block of code, to do it open with '{}', line " << current->line << std::endl;
            exit(21);
        }

        data.addFunctionToData(function);
    }
}