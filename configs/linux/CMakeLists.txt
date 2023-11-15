# linux
cmake_minimum_required(VERSION 3.16.3)
project(violence_linux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

# put output bin to ../../
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin)

add_executable(violence_linux ../../main.cpp
        ../../Reader.cpp
        ../../Reader.h
        ../../Token.cpp
        ../../Token.h
        ../../Lexer.cpp
        ../../Lexer.h
        ../../Interpreter.cpp
        ../../Interpreter.h
        ../../utils.h
        ../../Parser.cpp
        ../../Parser.h
)