#include "Double.h"

namespace JDD::Modules::Double {
    double max(double x, double y) {
        if (x > y)
            return x;
        return y;
    }

    double min(double x, double y) {
        if (x < y)
            return x;
        return y;
    }

    double sum(double x, double y) {
        return x + y;
    }

    double sub(double x, double y) {
        return x - y;
    }

    double mul(double x, double y) {
        return x * y;
    }

    double div(double x, double y) {
        return x / y;
    }

    double abs(double x) {
        return std::abs(x);
    }
}