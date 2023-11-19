# linux
cmake_minimum_required(VERSION 3.16.3)
project(violence_linux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

# make static
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")


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
        ../../InlineCHandler.cpp
        ../../InlineCHandler.h
)

# include dl
target_link_libraries(violence_linux dl)
target_link_libraries(violence_linux -static)