#pragma once

#include <iostream>
#include <map>
#include <optional>
#include "../Variable/Variable.h"
#include "../Function/Function.h"

namespace JDD::Definitions {
    class Data {
    public:
        std::map<std::string, Variable> Variables;
        std::map<std::string, Function> Functions;

        [[nodiscard]] bool isVariable(const std::string& name) const;
        std::optional<Variable> getVariableFromName(const std::string& name);
        void addVariableToData(const Variable& var);
        void addVariableToData(const std::string& name, const Value& value, const Types& type, const bool& isFinal);
        void removeVariableFromName(const std::string& name);
        void updateVariableValueFromData(const std::string& name, const std::string& value);

        [[nodiscard]] bool isFunction(const std::string& name) const;
        void addFunctionToData(const Function& func);
        std::optional<Function> getFunctionFromName(const std::string& name);
    };
}