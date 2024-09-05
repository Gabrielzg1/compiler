#include <iostream>
#include "Lexical/Lexical.h"
#include "Token/Token.h"

using namespace std;

int main() {
    try {
        Lexical lexer("code.txt");
        lexer.analyze();

        const auto& tokens = lexer.getTokens();
        for (const auto& token : tokens) {
            std::cout << token.toString() << std::endl;

        }

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}