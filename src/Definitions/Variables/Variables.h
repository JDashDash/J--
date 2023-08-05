#pragma once

#include <iostream>

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

        friend std::ostream& operator<<(std::ostream& flux, Variable const& var);
    };
}