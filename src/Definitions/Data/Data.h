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
        // Modules Enable
        bool isStringModuleImported;

        // Functions
        bool isVariable(const std::string& name);
        std::optional<Variable> getVariable(const std::string& name);
        void pushVariable(const Variable& var);
        void updateValueOfVariable(const std::string& name, const std::string& value);
    };
}