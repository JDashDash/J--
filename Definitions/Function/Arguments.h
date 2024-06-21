#pragma once

#include <iostream>
#include "../Types/Types.h"
#include "../Value/Value.h"

namespace JDD::Definitions {
    class Argument {
    public:
        Argument();
        Argument(Types t, std::string n);
        Argument(Types t, Value v, std::string n);

        std::string name;
        Types type;
        Value value;
    };
}