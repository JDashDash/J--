#include "Integer.h"

namespace JDD::Modules::Integer {
    int max(int x, int y) {
        if (x > y)
            return x;
        return y;
    }

    int min(int x, int y) {
        if (x < y)
            return x;
        return y;
    }

    int sum(int x, int y) {
        return x + y;
    }

    int sub(int x, int y) {
        return x - y;
    }

    int mul(int x, int y) {
        return x * y;
    }

    int div(int x, int y) {
        return x / y;
    }

    int fromBoolean(bool value) {
        if (value)
            return 1;
        return 0;
    }

    int abs(int x) {
        if (x > 0)
            return -x;
        return x;
    }
}