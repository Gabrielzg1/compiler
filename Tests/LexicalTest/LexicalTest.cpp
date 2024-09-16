// Tests/SymbolTableTest/SymbolTableTest.cpp
#include "../../Lexical/Lexical.h"
#include <iostream>

int main() {
    Lexical lexer("code.txt");
    lexer.analyze();

    const auto& tokens = lexer.getTokens();
    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;

    }

    return 0;
}