#include "ReturnVariable.h"

JDD::Definitions::ReturnVariable::ReturnVariable() = default;

JDD::Definitions::ReturnVariable::ReturnVariable(JDD::Definitions::Types t, JDD::Definitions::Value v) {
    this->type = t;
    this->value = v;
}
