// Tests/SymbolTableTest/SymbolTableTest.cpp
#include "../../SymbolTable/SymbolTable.h"
#include <iostream>

int main() {
    SymbolTable symbolTable; // Corrige a inicialização do objeto
    symbolTable.push("teste", 0, "var", 0);
    std::cout << symbolTable.peek()->name << std::endl; // Corrige o uso de std::endl
    return 0;
}