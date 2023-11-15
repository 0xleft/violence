# win_64 64
cmake_minimum_required(VERSION 3.16.3)
project(violence_win_64)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin)

add_executable(violence_win_64 ../../main.cpp
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