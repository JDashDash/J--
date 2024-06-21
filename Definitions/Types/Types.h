#pragma once

namespace JDD::Definitions {
    enum Types {
        INT,
        DOUBLE,
        BOOLEAN,
        STRING,
        VOID,
        FINAL_NotType
    };

    inline constexpr const char *typesString[] {
            "INT",
            "DOUBLE",
            "BOOLEAN",
            "STRING",
            "VOID",
            "FINAL_NotType"
    };
}