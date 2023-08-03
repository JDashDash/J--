#include "Data.h"

bool JDD::Definition::Data::isVariable(const std::string& name) {
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
