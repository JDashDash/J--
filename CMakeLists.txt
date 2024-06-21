cmake_minimum_required(VERSION 3.27)
project(jdashdash_modern_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(jdashdash_modern_parser main.cpp
        Lexer/Lexer.cpp
        Lexer/Lexer.h
        Parser/Parser.cpp
        Parser/Parser.h
        Definitions/Data/Data.cpp
        Definitions/Data/Data.h
        Definitions/Variable/Variable.cpp
        Definitions/Variable/Variable.h
        Definitions/Types/Types.h
        Definitions/Value/Value.h
        Definitions/Value/Value.cpp
        Expecter/Expecter.cpp
        Expecter/Expecter.h
        Definitions/Function/Function.cpp
        Definitions/Function/Function.h
        Definitions/Function/Arguments.cpp
        Definitions/Function/Arguments.h
        Definitions/Function/ReturnVariable.cpp
        Definitions/Function/ReturnVariable.h)
