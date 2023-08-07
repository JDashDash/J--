#include "Value.h"

std::ostream &JDD::Definition::operator<<(std::ostream &flux, const JDD::Definition::Value &v) {
    flux << "(Value: " << v.content << ", " << sAllTypes[v.type] << ")";
    return flux;
}
