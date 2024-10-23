// Tests/SymbolTableTest/SymbolTableTest.cpp
#include "../../SymbolTable/SymbolTable.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    SymbolTable symbolTable;
    symbolTable.push("x", "", "inteiro", "0x003");
    symbolTable.push("y", "", "inteiro", "0x002");
    // -x + 7 * (y div 3) = 10
    vector<string> input = {
            "-u", "x", "+", "7", "*", "y", "(", "div", "3", ")", ">", "10", "e", "nao", "falso"
    };

    vector<string> output = symbolTable.toPostFix(input);

    cout << "Posfixa: ";
    for (const string& token : output) {
        std::cout << token << " ";
    }


    std::cout << std::endl;
    try {
        string resultType = symbolTable.inferType(output);
        if (resultType == "inteiro") {
            cout << "O tipo da expressao: Inteiro." << endl;
        } else {
            cout << "O tipo da expressao: Booleano." << endl;
        }
    } catch (const exception& ex) {
        cout << "Erro: " << ex.what() << endl;
    }




    return 0;
}
