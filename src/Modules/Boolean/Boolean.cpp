#include "Boolean.h"

namespace JDD::Modules::Boolean {
    int toInt(bool value) {
        if (value)
            return 1;
        return 0;
    }

    bool fromInt(int x) {
        if (x == 1)
            return true;
        return false;
    }

    bool opposite(bool x) {
        if (x)
            return false;
        return true;
    }

    bool compare(int x, int y, const std::string& op) {
        if (op == "==") {
            return x == y;
        } else if (op == ">=") {
            return x >= y;
        } else if (op == "<=") {
            return x <= y;
        } else if (op == "!=") {
            return x != y;
        } else if (op == ">") {
            return x > y;
        } else if (op == "<") {
            return x < y;
        }
    }

    bool compare(double x, double y, const std::string& op) {
        if (op == "==") {
            return x == y;
        } else if (op == ">=") {
            return x >= y;
        } else if (op == "<=") {
            return x <= y;
        } else if (op == "!=") {
            return x != y;
        } else if (op == ">") {
            return x > y;
        } else if (op == "<") {
            return x < y;
        }
    }
}