#pragma once

#include <iostream>

namespace JDD::Definition {
    enum Types {
        INT,
        DOUBLE,
        BOOLEAN,
        STRING,
        VOID,
        FINAL_NotType
    };

    inline constexpr const char *sAllTypes[] {
            "INT",
            "DOUBLE",
            "BOOLEAN",
            "STRING",
            "VOID",
            "FINAL_NotType"
    };
}