// Tests/SymbolTableTest/SymbolTableTest.cpp
#include "../../SymbolTable/SymbolTable.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    SymbolTable symbolTable;

    // Teste 1: Empilhar alguns símbolos
    std::cout << "Pushing symbols onto the stack..." << std::endl;
    symbolTable.push("programName", "", "program", "0x001");
    symbolTable.push("var1", "", "var", "0x002");
    symbolTable.push("var2", "", "var", "0x003");
    symbolTable.push("proc1", "L", "proc", "0x004");
    symbolTable.push("var3", "", "var", "0x005");
    symbolTable.push("func1", "L", "function", "0x006");
    symbolTable.push("var1", "", "var", "0x002");
    symbolTable.push("var2", "", "var", "0x003");
    symbolTable.push("var3", "", "var", "0x002");
    symbolTable.push("var4", "", "var", "0x003");


    // Imprimir a pilha
    std::cout << "\nStack after pushing symbols:" << std::endl;
    symbolTable.printStack();

    // Teste 2: Verificar se as variáveis e funções estão na pilha
    std::cout << "\nChecking if 'var1' exists in the scope: " << (symbolTable.containsVar("var1") ? "True" : "False") << std::endl;
    std::cout << "Checking if 'func1' exists in the stack: " << (symbolTable.containsProcFunc("func1") ? "True" : "False") << std::endl;

    // Teste 3: Atribuir novo tipo às variáveis
    std::cout << "\nAssigning new type to variables (changing 'var' to 'int')..." << std::endl;
    symbolTable.assignTypeToVariables("int");

    // Imprimir a pilha novamente para ver as mudanças
    std::cout << "\nStack after assigning new types to variables:" << std::endl;
    symbolTable.printStack();

    // Teste 4: Atribuir novo tipo a funções
    std::cout << "\nAssigning new type to functions (changing 'func' to 'void')..." << std::endl;
    symbolTable.assignTypeToFunction("void");

    std::cout << "\nStack after assigning new types to functions:" << std::endl;
    symbolTable.printStack();

    // Teste 5: Fazer a poda da pilha, removendo até o primeiro nível de escopo 'L'
    std::cout << "\nPruning the stack until the first 'L' scope level..." << std::endl;
    symbolTable.cutStack();

    std::cout << "\nStack after first pruning:" << std::endl;
    symbolTable.printStack();

    // Teste 6: Verificar estado após poda da pilha
    std::cout << "\nChecking if 'var1' still exists after pruning: " << (symbolTable.containsVar("var1") ? "True" : "False") << std::endl;
    std::cout << "Checking if 'func1' still exists after pruning: " << (symbolTable.containsProcFunc("func1") ? "True" : "False") << std::endl;

    std::cout << "Type of 'func1' is: " << symbolTable.getFuncType("func1") << std::endl;



    // Teste 7: Testar pilha vazia
    std::cout << "\nPopping all elements to test empty stack..." << std::endl;
    while (!symbolTable.isEmpty()) {
        symbolTable.pop();
    }
    std::cout << "Stack after popping all elements:" << std::endl;
    symbolTable.printStack();
    std::cout << "Is stack empty? " << (symbolTable.isEmpty() ? "Yes" : "No") << std::endl;

    // Teste 8: Verificar comportamento de escopo vazio
    std::cout << "\nPushing symbols with scope and verifying containsVar behavior in local scope..." << std::endl;
    symbolTable.push("globalVar", "", "var", "0x010");
    symbolTable.push("localVar", "L", "var", "0x011");
    std::cout << "Checking if 'localVar' exists in the local scope: " << (symbolTable.containsVar("localVar") ? "True" : "False") << std::endl;
    symbolTable.cutStack();
    std::cout << "Checking if 'localVar' exists after scope cut: " << (symbolTable.containsVar("localVar") ? "True" : "False") << std::endl;


    cout << endl << endl;

    // -x + 7 * (y div 3) > 10
    vector<string> input = {
            "-u", "x", "+", "7", "*", "y", "(", "div", "3", ")", "=", "10"
    };

    vector<string> output = symbolTable.toPostFix(input);

    cout << "Posfixa: ";
    for (const string& token : output) {
        std::cout << token << " ";
    }

    std::cout << std::endl;
    try {
        Type resultType = symbolTable.inferType(output);
        if (resultType == Type::Integer) {
            cout << "O tipo da expressão é Inteiro." << endl;
        } else {
            cout << "O tipo da expressão é Booleano." << endl;
        }
    } catch (const exception& ex) {
        cout << "Erro: " << ex.what() << endl;
    }




    return 0;
}
