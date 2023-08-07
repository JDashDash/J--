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
    void useStringModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, Definition::Value& value);
    void useBooleanModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, Definition::Value& value);
    void useDoubleModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, Definition::Value& value);
    void useIntegerModule(std::vector<JDD::Lexer::Token>::const_iterator& current, JDD::Definition::Data& data, Definition::Value& value);
}