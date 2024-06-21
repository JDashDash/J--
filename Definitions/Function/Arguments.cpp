#include "Arguments.h"

JDD::Definitions::Argument::Argument() = default;

JDD::Definitions::Argument::Argument(JDD::Definitions::Types t, JDD::Definitions::Value v, std::string n) {
    this->type = t;
    this->value = v;
    this->name = n;
}

JDD::Definitions::Argument::Argument(JDD::Definitions::Types t, std::string n) {
    this->type = t;
    this->name = n;
}
