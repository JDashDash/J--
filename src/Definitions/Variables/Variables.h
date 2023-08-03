#pragma once

#include <iostream>

#include "../Types/Types.h"
#include "../Value/Value.h"

namespace JDD::Definition {
    class Variable {
    public:
        Variable();
        Variable(std::string name, Value value, Types type);
        Variable(std::string name, Value value, Types type, bool isFinal);

        std::string name;
        Value value;
        Types type;
        bool isFinal = false;
    };
}