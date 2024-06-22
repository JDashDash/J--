#pragma once

#include <iostream>
#include <vector>
#include "../Lexer/Lexer.h"
#include "../Definitions/Data/Data.h"
#include "../Definitions/Variable/Variable.h"
#include "../Definitions/Value/Value.h"
#include "../Expecter/Expecter.h"
#include "../Definitions/Function/Function.h"
#include "../Definitions/Function/Arguments.h"
#include "../Definitions/Function/ReturnVariable.h"

using namespace JDD::Definitions;
using namespace JDD::Lexer;

namespace JDD::Parser {
    void run(std::vector<Token>& tokensList);
    void runCodeBlock(std::vector<JDD::Lexer::Token>& tokenBlock, Data& globalData, std::optional<Function>& function);
    bool instructionsManagement(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, std::optional<Function>& function);

    void print(Data &data, std::vector<JDD::Lexer::Token> &tokensList, std::vector<Token>::iterator &current, bool jumpLine, std::optional<Function>& function);
    void basicVariableDeclaration(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, JDD::Definitions::Types type, std::optional<Function>& function);
    void managementVariablesFunctionsCallModification(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, Token& id);
    void deleteVariableOrFunction(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current);
    void assert_eq(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current);
    void functionDeclaration(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, JDD::Definitions::FunctionState state);
    void returnFromFunction(Data& data, std::vector<JDD::Lexer::Token>& tokensList, std::vector<Token>::iterator& current, std::optional<Function>& function);
}