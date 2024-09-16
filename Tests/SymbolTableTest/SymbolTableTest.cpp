// Tests/SymbolTableTest/SymbolTableTest.cpp
#include "../../SymbolTable/SymbolTable.h"
#include <iostream>

using namespace std;
int main() {
    SymbolTable symbolTable;
    symbolTable.push("teste", 0, "var", 0);
    cout << "Nome da variavel: " << symbolTable.peek()->name << endl;
    cout <<  "Tipo anterior: " << symbolTable.peek()->type << endl;
    symbolTable.assignTypeToVariables("int");
    cout << "Tipo posterior: " << symbolTable.peek()->type << endl;

    return 0;
}