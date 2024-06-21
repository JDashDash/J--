#pragma once

#include <iostream>
#include <optional>
#include <string_view>
#include "../Lexer/Lexer.h"
#include "../Definitions/Data/Data.h"
#include "../Definitions/Variable/Variable.h"
#include "../Definitions/Value/Value.h"
#include "../Definitions/Types/Types.h"

namespace JDD::Parser {
    std::optional<JDD::Lexer::Token> ExpectIdentifiant(std::vector<JDD::Lexer::Token>::iterator& current);
    std::optional<JDD::Definitions::Value> ExpectValue(std::vector<JDD::Lexer::Token>::iterator& current);
    std::optional<JDD::Definitions::Types> ExpectType(std::vector<JDD::Lexer::Token>::iterator& current);
    std::optional<JDD::Lexer::Token> ExpectOperator(std::vector<JDD::Lexer::Token>::iterator& current, std::string_view o = "");
    std::optional<std::string> ExpectConditionalOperator(std::vector<JDD::Lexer::Token>::iterator& current);
    JDD::Definitions::Value ReturnFinalValueFromListToken(JDD::Definitions::Data& data, std::vector<JDD::Lexer::Token>::iterator& current, std::vector<JDD::Lexer::Token>& list, JDD::Definitions::Types& awaitingType);
}