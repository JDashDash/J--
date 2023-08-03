#include "String.h"

namespace JDD::Modules::String {
    std::string concat(const std::string& str1, const std::string& str2) {
        return str1 + str2;
    }

    int getIndexFromChar(const std::string& str, char x) {
        if (str.find(x) == std::string::npos) {
            return -1;
        }
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == x) {
                return i;
            }
        }
        return -1;
    }

    char getCharFromIndex(const std::string& str, int index) {
        if (index < 0 || index >= str.length()) {
            return '\0';
        }
        return str[index];
    }
}