#pragma once

#include <iostream>
#include <map>
#include <optional>

#include "../Variables/Variables.h"

namespace JDD::Definition {
    class Data {
    private:
        std::map<std::string, Variable> Variables; // Variable Name, Variable Object

    public:
        bool isVariable(const std::string& name);
        std::optional<Variable> getVariable(const std::string& name);
        void pushVariable(const Variable& var);
    };
}