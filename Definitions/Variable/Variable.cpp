#include "Variable.h"

namespace JDD::Definitions {
    Variable::Variable() = default;

    Variable::Variable(std::string name, JDD::Definitions::Value value, JDD::Definitions::Types type) {
        this->name = name;
        this->value = value;
        this->type = type;
    }

    Variable::Variable(std::string name, JDD::Definitions::Value value, JDD::Definitions::Types type,
                                         bool isFinal) {
        this->name = name;
        this->value = value;
        this->type = type;
        this->isFinal = isFinal;
    }

    std::ostream& operator<<(std::ostream& flux, Variable const& v) {
        flux << " [VARIABLE OBJECT] >>> " << v.name << " | " << JDD::Definitions::typesString[v.type] << " | " << v.value.content << " | " << v.isFinal << " (0 true, 1 false)" ;
        return flux;
    }

    bool Variable::contains_fileAllowAccess(const std::string &elem) {
        if(std::find(this->fileAllowAccess.begin(), this->fileAllowAccess.end(), elem) != this->fileAllowAccess.end())
        {
            return true;
        }
        return false;
    }
}