#include "Function.h"

bool JDD::Definitions::Function::isCallable(const std::string &elem) {
    bool result = false;
    if(std::find(this->filesAllowAccess.begin(), this->filesAllowAccess.end(), elem) != this->filesAllowAccess.end())
    {
        result = true;
    }
    return result;
}

bool JDD::Definitions::Function::isArgument(const std::string& n) const {
    if (this->arguments.contains(n))
        return true;
    return false;
}

void JDD::Definitions::Function::pushArgument(const JDD::Definitions::Argument &a) {
    this->arguments[a.name] = a;
}

int JDD::Definitions::Function::sizeArgumentsNeeded() {
    return this->arguments.size();
}