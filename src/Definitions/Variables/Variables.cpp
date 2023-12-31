#include "Variables.h"

#include <utility>

JDD::Definition::Variable::Variable() = default;

JDD::Definition::Variable::Variable(std::string name, JDD::Definition::Value value, JDD::Definition::Types type) {
    this->name = std::move(name);
    this->type = type;
    this->value = std::move(value);
}

JDD::Definition::Variable::Variable(std::string name, JDD::Definition::Value value, JDD::Definition::Types type, bool isFinal) {
    this->name = std::move(name);
    this->type = type;
    this->value = std::move(value);
    this->isFinal = isFinal;
}

std::ostream &JDD::Definition::operator<<(std::ostream &flux, const JDD::Definition::Variable &var) {
    flux << "(Variable: " << var.name << ", " << var.value.content << ", " << var.isFinal << ")";
    return flux;
}

bool JDD::Definition::Variable::contains_fileAllowAccess(const std::string &elem) {
    bool result = false;
    if(std::find(this->fileAllowAccess.begin(), this->fileAllowAccess.end(), elem) != this->fileAllowAccess.end())
    {
        result = true;
    }
    return result;
}
