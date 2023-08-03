#pragma once

#include <string>

namespace JDD::Modules::String {
    std::string concat(const std::string& str1, const std::string& str2);
    int getIndexFromChar(const std::string& str, char x);
    char getCharFromIndex(const std::string& str, int index);
}
