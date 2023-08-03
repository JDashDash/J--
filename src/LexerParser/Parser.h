#pragma once

#include <iostream>
#include "Lexer.h"

#include "../Expecter/Expecter.h"
#include "../Definitions/Data/Data.h"
#include "../Definitions/Types/Types.h"
#include "../Definitions/Variables/Variables.h"

namespace JDD::Parser {
    class JDDParser {
    public:
        static void main(const std::vector<JDD::Lexer::Token>& tokenList);

    private:
        static bool ManagerInstruction(std::vector<Lexer::Token>::const_iterator& current, Definition::Data& data);

        // Instructions :
        static void print(std::vector<Lexer::Token>::const_iterator &current, bool jumpLine, Definition::Data& data);
        static void variables(std::vector<Lexer::Token>::const_iterator &current, JDD::Definition::Types type, Definition::Data& data);
    };
}