#pragma once

#include <iostream>

#include "../Types/Types.h"

namespace JDD::Definition {
    class Value {
    public:
        std::string content;
        JDD::Definition::Types type;
    };
}