#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "../Value/Value.h"

namespace JDD::Definitions {
    enum VariableState {
        VarPublic,
        VarPrivate,
        VarProtected
    };

    inline constexpr const char *variableStateString[] {
            "Public",
            "Private",
            "Protected"
    };

    class Variable {
    public:
        Variable();
        Variable(std::string name, Value value, Types type);
        Variable(std::string name, Value value, Types type, bool isFinal);

        bool contains_fileAllowAccess(const std::string & elem);
        friend std::ostream& operator<<(std::ostream& flux, Variable const& v);

        std::string name;
        Value value;
        Types type;
        bool isFinal = false;
        VariableState state = VarPrivate;
        std::vector<std::string> fileAllowAccess;
        std::string possibleReference_name;
    };

}