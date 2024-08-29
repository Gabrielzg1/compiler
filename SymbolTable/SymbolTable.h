#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
using namespace std;

// Estrutura que armazena as informações do símbolo
struct SymbolInfo {
    string name;
    int scopeLevel;
    string type;
    int memoryAddress;
};

// Estrutura do nó da pilha
struct Node {
    SymbolInfo* symbolInfo;
    Node* next;
};

class SymbolTable {
public:
    SymbolTable();
    void push(SymbolInfo* symbolInfo);
    void pop();
    bool isEmpty() const;
    SymbolInfo* peek() const;

private:
    Node* top;  // Ponteiro para o topo da pilha
};

#endif // SYMBOL_TABLE_H