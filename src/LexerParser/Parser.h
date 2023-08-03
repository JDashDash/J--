#pragma once

#include <iostream>
#include <valarray>
#include <functional>
#include <memory>

#include "Lexer.h"

#include "../Expecter/Expecter.h"
#include "../Definitions/Data/Data.h"
#include "../Definitions/Types/Types.h"
#include "../Definitions/Variables/Variables.h"

#include "../Modules/ModuleManager.h"

namespace JDD::Parser {
    class JDDParser {
    public:
        static void main(const std::vector<JDD::Lexer::Token>& tokenList);

    private:
        // Essentials:
        static bool ManagerInstruction(std::vector<Lexer::Token>::const_iterator& current, Definition::Data& data);

        // Instructions :
        static void print(std::vector<Lexer::Token>::const_iterator &current, bool jumpLine, Definition::Data& data);
        static void variables(std::vector<Lexer::Token>::const_iterator &current, JDD::Definition::Types type, Definition::Data& data);
        static void import(std::vector<Lexer::Token>::const_iterator &current, Definition::Data& data);

        // Special :
        static void updateVariableValue(std::vector<Lexer::Token>::const_iterator &current, Definition::Data& data, const std::string& var_name);
    };
}