#pragma once

#include <iostream>

#include "../Types/Types.h"

namespace JDD::Definition {
    class Value {
    public:
        std::string content;
        JDD::Definition::Types type;
        std::string variableName = "";

        friend std::ostream& operator<<(std::ostream& flux, Value const& v);
    };
}