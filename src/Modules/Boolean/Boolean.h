#pragma once

#include <iostream>

namespace JDD::Modules::Boolean {
    int toInt(bool value);
    bool fromInt(int x);
    bool opposite(bool x);
    bool compare(int x, int y, const std::string& op);
    bool compare(double x, double y, const std::string& op);
}