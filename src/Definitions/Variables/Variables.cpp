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
