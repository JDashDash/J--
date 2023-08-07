#include "Data.h"

bool JDD::Definition::Data::isVariable(const std::string& name) const {
    if (this->Variables.contains(name))
        return true;
    return false;
}

std::optional<JDD::Definition::Variable> JDD::Definition::Data::getVariable(const std::string &name) {
    if (isVariable(name))
        return this->Variables[name];
    return std::nullopt;
}

void JDD::Definition::Data::pushVariable(const JDD::Definition::Variable& var) {
    this->Variables[var.name] = var;
}

void JDD::Definition::Data::updateValueOfVariable(const std::string &name, const std::string &value) {
    if (this->isVariable(name)) {
        auto var = this->getVariable(name);
        this->Variables[var->name].value.content = value;
    }
}

void JDD::Definition::Data::pushFunction(const JDD::Definition::Function &func) {
    this->Functions[func.name] = func;
}

bool JDD::Definition::Data::isFunction(const std::string &name) const {
    if (this->Functions.contains(name))
        return true;
    return false;
}

std::optional<JDD::Definition::Function> JDD::Definition::Data::getFunction(const std::string &name) {
    if (isFunction(name))
        return this->Functions[name];
    return std::nullopt;
}
