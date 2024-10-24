// Tests/SymbolTableTest/SymbolTableTest.cpp
#include "../../SymbolTable/SymbolTable.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    SymbolTable symbolTable;

    // Inserindo alguns símbolos na tabela para testar
    symbolTable.push("x", "", "inteiro", "0x003");
    symbolTable.push("y", "", "inteiro", "0x002");
    symbolTable.push("z", "", "booleano", "0x004");

    // Teste de `getType`
    std::cout << "Teste de getType:" << std::endl;

    std::string typeX = symbolTable.getType("x");
    std::cout << "Tipo de 'x': " << (typeX.empty() ? "Não encontrado" : typeX) << std::endl;

    std::string typeY = symbolTable.getType("y");
    std::cout << "Tipo de 'y': " << (typeY.empty() ? "Não encontrado" : typeY) << std::endl;

    std::string typeZ = symbolTable.getType("z");
    std::cout << "Tipo de 'z': " << (typeZ.empty() ? "Não encontrado" : typeZ) << std::endl;

    std::string typeW = symbolTable.getType("w");
    std::cout << "Tipo de 'w' (não existe): " << (typeW.empty() ? "Não encontrado" : typeW) << std::endl;

    // Teste de expressão pós-fixa
    // -x + 7 * (y div 3)
    std::vector<std::string> input = {
            "-u", "x", "+", "7", "*", "y", "(", "div", "3", ")"
    };

    std::vector<std::string> output = symbolTable.toPostFix(input);

    std::cout << "Posfixa: ";
    for (const std::string& token : output) {
        std::cout << token << " ";
    }

    std::cout << std::endl;
    try {
        std::string resultType = symbolTable.inferType(output);
        if (resultType == "inteiro") {
            std::cout << "O tipo da expressao: Inteiro." << std::endl;
        } else {
            std::cout << "O tipo da expressao: Booleano." << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cout << "Erro: " << ex.what() << std::endl;
    }

    return 0;
}
