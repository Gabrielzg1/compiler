#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
using namespace std;

// Estrutura que armazena as informações do símbolo
struct SymbolInfo {
    string name;
    string scopeLevel;
    string type;
    string memoryAddress;
};

// Estrutura do nó da pilha
struct Node {
    SymbolInfo* symbolInfo;
    Node* next;
};

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();
    void push(string name, string scopeLevel, string type, string memoryAddress);
    void pop();
    bool isEmpty() const;
    SymbolInfo* peek() const;
    bool containsVar(std::string name) const;
    bool containsProcFunc(std::string name) const;
    void assignTypeToVariables(const std::string& newType);
    void printStack() const;
    void cutStack();
    void assignTypeToFunction(const std::string& newType);

private:
    Node* top;  // Ponteiro para o topo da pilha
    //std::vector<std::string> toPostFix(const std::vector<std::string>& input); // conversor de in Fix to post Fix

};

#endif // SYMBOL_TABLE_H