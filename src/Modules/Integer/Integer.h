#pragma once

#include <iostream>

namespace JDD::Modules::Integer {
    int max(int x, int y);
    int min(int x, int y);
    int sum(int x, int y); // x + y
    int sub(int x, int y); // x - y
    int mul(int x, int y); // x * y
    int div(int x, int y); // x * y
    int fromBoolean(bool value); //1 -> true, 0 -> false
    int abs(int x); // absolute value
}