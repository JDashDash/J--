#include "Functions.h"

bool JDD::Definition::Function::isArgument(const std::string& n) const {
    if (this->arguments.contains(n))
        return true;
    return false;
}

void JDD::Definition::Function::pushArgument(const JDD::Definition::Argument& a) {
    this->arguments[a.name] = a;
}

JDD::Definition::Argument::Argument() = default;

JDD::Definition::Argument::Argument(std::string n, JDD::Definition::Types t) {
    this->name = n;
    this->type = t;
}
