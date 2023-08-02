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
            print(current, false);
            return true;
        }  if (instruction.has_value() && instruction->content == "println") {
            print(current, true);
            return true;
        }
        return false;
    }

    void JDDParser::print(std::vector<Lexer::Token>::const_iterator &current, bool jumpLine) {
        auto openValues = ExpectOperator(current, "(");
        if (!openValues.has_value()) {
            std::cout << "forgot to open '(' and make sure to close with ')'" << std::endl;
        }

        auto value = ExpectValue(current);
        if (!value.has_value()) {
            std::cout << "forgot to give value in print" << std::endl;
        }

        auto closeValues = ExpectOperator(current, ")");
        if (!closeValues.has_value()) {
            std::cout << "forgot to close with ')'" << std::endl;
        }

        auto endInstruction = ExpectOperator(current, ";");
        if (!endInstruction.has_value()) {
            std::cout << "forgot to close the instruction with ';'" << std::endl;
        }

        if (jumpLine) { std::cout << "\n"; }
        std::cout << value->content;
    }
}