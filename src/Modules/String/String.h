#pragma once

#include <string>
#include <algorithm>

namespace JDD::Modules::String {
    std::string concat(const std::string& str1, const std::string& str2);
    int getIndexFromChar(const std::string& str, char x);
    char getCharFromIndex(const std::string& str, int index);
    bool equals(const std::string& str1, const std::string& str2);
    bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
    bool isEmpty(const std::string& str1);
    std::string toLowerCase(const std::string& str1);
    std::string toUpperCase(const std::string& str1);
    std::string replace(const std::string& str1, char x, char y);
    std::string valueOf(std::string &string);
}
