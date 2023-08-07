#pragma once

#include <iostream>
#include <map>
#include <optional>

#include "../Variables/Variables.h"
#include "../Functions/Functions.h"

namespace JDD::Definition {
    class Data {
    public:
        std::map<std::string, Variable> Variables; // Variable Name, Variable Object
        std::map<std::string, Function> Functions; // Function Name, Function Object

        // Modules Enable for Data class
        bool isStringModuleImported = false;
        bool isBooleanModuleImported = false;
        bool isDoubleModuleImported = false;
        bool isIntegerModuleImported = false;

        // Functions for Data class
        [[nodiscard]] bool isVariable(const std::string& name) const;
        std::optional<Variable> getVariable(const std::string& name);
        void pushVariable(const Variable& var);
        void updateValueOfVariable(const std::string& name, const std::string& value);
        void pushFunction(const Function& func);
        [[nodiscard]] bool isFunction(const std::string& name) const;
        std::optional<Function> getFunction(const std::string& name);
    };
}