#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <stack>
#include <optional>
#include <iostream>

// Estrutura que armazena as informações do símbolo
struct SymbolInfo {
    std::string name;
    int scopeLevel;
    std::string type;
    int memoryAddress;
};

class SymbolTable {
public:
    void enterScope();
    void leaveScope();
    bool addSymbol(const std::string& name, const std::string& type, int memoryAddress);
    std::optional<SymbolInfo> getSymbolInfo(const std::string& name) const;
    void setTypeForVariables(const std::string& type);
    void displayStack() const;

private:
    std::stack<std::unordered_map<std::string, SymbolInfo>> table;
};

#endif // SYMBOL_TABLE_H
