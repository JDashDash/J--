#pragma once

#include <iostream>
#include <valarray>
#include <functional>
#include <memory>
#include <vector>

#include "Lexer.h"

#include "../Expecter/Expecter.h"
#include "../Definitions/Data/Data.h"
#include "../Definitions/Types/Types.h"
#include "../Definitions/Variables/Variables.h"
#include "../Definitions/Functions/Functions.h"

#include "../Modules/ModuleManager.h"

namespace JDD::Parser {
    class JDDParser {
    public:
        static void main(const std::vector<JDD::Lexer::Token>& tokenList);

    private:
        // Essentials:
        static bool ManagerInstruction(std::vector<Lexer::Token>::const_iterator& current, Definition::Data& data, std::vector<Lexer::Token> tokenList);

        // Instructions :
        static void print(std::vector<Lexer::Token>::const_iterator &current, bool jumpLine, Definition::Data& data);
        static void variables(std::vector<Lexer::Token>::const_iterator &current, JDD::Definition::Types type, Definition::Data& data, Definition::VariableState state);
        static void import(std::vector<Lexer::Token>::const_iterator &current, Definition::Data& data);

        // Special :
        static void updateVariableValue(std::vector<Lexer::Token>::const_iterator &current, Definition::Data& data, const std::string& var_name);

        static void
        specialVariable_defineFunction(std::vector<Lexer::Token>::const_iterator &current, Definition::Data &data,
                                       std::vector<Lexer::Token> tokenList, Definition::FunctionState state);
    };
}