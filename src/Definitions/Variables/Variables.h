#pragma once

#include <iostream>
#include <vector>
#include<algorithm>

#include "../Types/Types.h"
#include "../Value/Value.h"

namespace JDD::Definition {
    enum VariableState {
            VarPublic,
            VarPrivate,
            VarProtected
    };

    inline constexpr const char *sAllVariableState[] {
            "Public",
            "Private",
            "Protected"
    };

    class Variable {
    public:
        Variable();
        Variable(std::string name, Value value, Types type);
        Variable(std::string name, Value value, Types type, bool isFinal);

        std::string name;
        Value value;
        Types type;
        bool isFinal = false;
        VariableState state = VarPrivate;
        std::vector<std::string> fileAllowAccess;
        std::string possibleReference_name = "";

        bool contains_fileAllowAccess(const std::string & elem);

        friend std::ostream& operator<<(std::ostream& flux, Variable const& var);
    };
}