cmake_minimum_required(VERSION 3.28)
project(compiler)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

# Main application executable
add_executable(compiler
        main.cpp
        Token/Token.cpp
        Token/Token.h
        Lexical/Lexical.cpp
        Lexical/Lexical.h
        SymbolTable/SymbolTable.cpp
        SymbolTable/SymbolTable.h
        Tests/LexicalTest/LexicalTest.cpp
)

# Test executable
add_executable(test_symbol_table
        Tests/SymbolTableTest/SymbolTableTest.cpp
        SymbolTable/SymbolTable.cpp
        SymbolTable/SymbolTable.h
)

add_executable(
        lexical_test Tests/LexicalTest/LexicalTest.cpp
        Lexical/Lexical.cpp
        Lexical/Lexical.h
        Token/Token.cpp
        Token/Token.h
)
