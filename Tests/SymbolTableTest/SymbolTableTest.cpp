// Tests/SymbolTableTest/SymbolTableTest.cpp
#include "../../SymbolTable/SymbolTable.h"
#include <iostream>

int main() {
    SymbolTable symbolTable;

    // Empilhar alguns símbolos
    std::cout << "Pushing symbols onto the stack..." << std::endl;
    symbolTable.push("Nome do programa", "", "programa", "");
    symbolTable.push("var1", "", "var", "");
    symbolTable.push("var2", "", "var", "");
    symbolTable.push("proc1", "L", "proc", "");
    symbolTable.push("var3", "", "var", "");
    symbolTable.push("func1", "L", "func", "");
    symbolTable.push("var6", "", "var", "");
    symbolTable.push("var7", "", "var", "");
    symbolTable.push("var8", "", "var", "");
    symbolTable.push("var9", "", "var", "");

    // Imprimir a pilha atual
    std::cout << "Stack after pushing symbols:" << std::endl;
    symbolTable.printStack();

    // Verificar se algumas variáveis e funções estão na pilha
    std::cout << "\nChecking if 'var1' exists in the scope: " << symbolTable.containsVar("var1") << std::endl;
    std::cout << "Checking if 'func1' exists in the stack: " << symbolTable.containsProcFunc("func1") << std::endl;

    // Atribuir um novo tipo às variáveis
    std::cout << "\nAssigning new type to variables (changing 'var' to 'int')..." << std::endl;
    symbolTable.assignTypeToVariables("int");

    // Imprimir a pilha novamente para ver as mudanças
    std::cout << "\nStack after assigning new types to variables:" << std::endl;
    symbolTable.printStack();

    // Fazer a poda da pilha, removendo elementos até encontrar o "L"
    std::cout << "\nPruning the stack until the first 'L' scope level..." << std::endl;
    symbolTable.cutStack();

    // Imprimir a pilha novamente após a poda
    std::cout << "\nStack after pruning:" << std::endl;
    symbolTable.printStack();

    return 0;
}
