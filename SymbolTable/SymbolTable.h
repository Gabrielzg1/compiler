#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
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
    vector<string> toPostFix(const vector<string>& input);
    string inferType(const vector<string>& postFixExpr);
    bool isProcedureOrProgram(const std::string& name);
    string getType(const std::string& name);



private:
    Node* top;  // Ponteiro para o topo da pilha


};

#endif // SYMBOL_TABLE_H