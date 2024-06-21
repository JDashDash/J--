#include "Value.h"

namespace JDD::Definitions {
    std::ostream &operator<<(std::ostream &flux, const Value &v) {
        flux << " [VALUE OBJECT] >>> " << v.content << " | " << JDD::Definitions::typesString[v.type] << " | " << v.variableName ;
        return flux;
    }
}