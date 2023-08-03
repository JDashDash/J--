cmake_minimum_required(VERSION 3.26)
project(JDashDash)

set(CMAKE_CXX_STANDARD 23)

add_executable(JDashDash
        main.cpp
        src/LexerParser/Lexer.cpp
        src/LexerParser/Lexer.h
        src/LexerParser/Parser.cpp
        src/LexerParser/Parser.h
        src/Definitions/Data/Data.cpp
        src/Definitions/Data/Data.h
        src/Expecter/Expecter.cpp
        src/Expecter/Expecter.h
        src/Definitions/Types/Types.cpp
        src/Definitions/Types/Types.h
        src/Definitions/Value/Value.cpp
        src/Definitions/Value/Value.h
        src/Definitions/Variables/Variables.cpp
        src/Definitions/Variables/Variables.h
)
