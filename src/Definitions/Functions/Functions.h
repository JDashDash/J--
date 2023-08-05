#include <iostream>
#include <map>
#include "../Types/Types.h"
#include "../Value/Value.h"
#include "../../LexerParser/Lexer.h"

namespace JDD::Definition {
    enum FunctionState {
        FuncPrivate,
        FuncPublic,
        FuncProtected,
    };

    inline constexpr const char *sAllFunctionState[] {
            "Private",
            "Public",
            "Protected"
    };

    class Argument {
    public:
        Argument();
        Argument(std::string n, Types t);

        std::string name;
        Types type;
        Value value;
    };

    class Function {
    public:
        std::string name;
        Types type;
        FunctionState state;
        std::map<std::string, Argument> arguments;
        Value returnValue;
        std::vector<Lexer::Token> tokens;

        [[nodiscard]] bool isArgument(const std::string& n) const;
        void pushArgument(const Argument& a);
    };
}