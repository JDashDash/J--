#pragma once

#include <iostream>
#include "../Types/Types.h"
#include "../Value/Value.h"

namespace JDD::Definitions {
    class ReturnVariable {
    public:
        ReturnVariable();
        ReturnVariable(Types t, Value v);

        Types type;
        Value value;
    };
}