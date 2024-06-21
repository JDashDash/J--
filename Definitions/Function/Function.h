#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "../Types/Types.h"
#include "../Value/Value.h"
#include "../../Lexer/Lexer.h"
#include "Arguments.h"
#include "ReturnVariable.h"

namespace JDD::Definitions {
    enum FunctionState {
        FuncPrivate,
        FuncPublic,
        FuncProtected
    };

    inline constexpr const char *sAllFunctionState[] {
            "Private",
            "Public",
            "Protected"
    };

    class Function {
    public:
        std::string name;
        ReturnVariable returnVariable;
        FunctionState state;
        std::map<std::string, Argument> arguments;
        std::vector<Lexer::Token> content_tokens;
        std::vector<std::string> filesAllowAccess;

        bool isCallable(const std::string& elem);
        [[nodiscard]] bool isArgument(const std::string& n) const;
        void pushArgument(const Argument& a);
        int sizeArgumentsNeeded();
    };
}