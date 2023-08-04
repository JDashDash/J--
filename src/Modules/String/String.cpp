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

    bool equals(const std::string& str1, const std::string& str2) {
        return str1 == str2;
    }

    bool equalsIgnoreCase(const std::string& str1, const std::string& str2) {
        std::string s1 = str1;
        std::string s2 = str2;
        std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
        std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
        return s1 == s2;
    }

    bool isEmpty(const std::string& str1) {
        if (str1.length() > 0) return false;
        return true;
    }

    std::string toLowerCase(const std::string& str1) {
        std::string s1 = str1;
        std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
        return s1;
    }

    std::string toUpperCase(const std::string& str1) {
        std::string s = str1;
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); }
        );
        return s;
    }

    std::string replace(const std::string& str1, char x, char y) {
        std::string s = str1;
        for (char & i : s) {
            if (i == x) {
                i = y;
            }
        }
        return s;
    }

    std::string valueOf(std::string& string) {
        return string;
    }
}