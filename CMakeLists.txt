cmake_minimum_required(VERSION 3.28)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cpp
        Token/Token.cpp
        Token/Token.h
        Lexical/Lexical.cpp
        Lexical/Lexical.h
        SymbolTable/SymbolTable.cpp
        SymbolTable/SymbolTable.h
)
