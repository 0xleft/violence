cmake_minimum_required(VERSION 3.16.3)
project(violence)

set(CMAKE_CXX_STANDARD 17)

add_executable(violence main.cpp
        Reader.cpp
        Reader.h
        Token.cpp
        Token.h
        Lexer.cpp
        Lexer.h
        Interpreter.cpp
        Interpreter.h
        utils.h
        Parser.cpp
        Parser.h
        AST.cpp
        AST.h
)
