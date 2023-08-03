#pragma once

#include <vector>

#include "../LexerParser/Lexer.h"
#include "../Expecter/Expecter.h"
#include "../Definitions/Data/Data.h"
#include "../Definitions/Types/Types.h"
#include "../Definitions/Value/Value.h"
#include "../Definitions/Variables/Variables.h"
#include "String/String.h"

namespace JDD::Modules::ModulesManager {
    void useStringModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, std::string& content, std::optional<JDD::Definition::Value>& value);
}