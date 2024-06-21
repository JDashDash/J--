#pragma once

#include <iostream>
#include "../Types/Types.h"

namespace JDD::Definitions {
    class Value {
    public:
        std::string content;
        JDD::Definitions::Types type;
        std::string variableName;

        friend std::ostream& operator<<(std::ostream& flux, Value const& v);
    };
}