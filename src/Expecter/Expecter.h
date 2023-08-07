#pragma once

#include <iostream>
#include <optional>
#include <string_view>
#include "../LexerParser/Lexer.h"
#include "../Definitions/Types/Types.h"
#include "../Definitions/Value/Value.h"
#include "../Definitions/Data/Data.h"

std::optional<JDD::Lexer::Token> ExpectIdentifiant(std::vector<JDD::Lexer::Token>::const_iterator& current);
std::optional<JDD::Lexer::Token> ExpectOperator(std::vector<JDD::Lexer::Token>::const_iterator& current, std::string_view o = "");
std::optional<JDD::Definition::Types> ExpectType(std::vector<JDD::Lexer::Token>::const_iterator& current);
std::optional<JDD::Definition::Value> ExpectValue(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data);