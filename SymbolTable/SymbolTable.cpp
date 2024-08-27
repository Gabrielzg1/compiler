#include "SymbolTable.h"

// Entra em um novo escopo
void SymbolTable::enterScope() {
    table.push(std::unordered_map<std::string, bool>());
}

// Sai de um escopo
void SymbolTable::leaveScope() {
    if (!table.empty()) {
        table.pop();
    }
}

// Adiciona um símbolo ao escopo atual
bool SymbolTable::addSymbol(const std::string& name) {
    if (!table.empty()) {
        table.top()[name] = true;
        return true;
    }
    return false;
}

// Verifica se um símbolo é visível no escopo atual
bool SymbolTable::isSymbolVisible(const std::string& name) const {
    std::stack<std::unordered_map<std::string, bool>> tempStack = table;
    while (!tempStack.empty()) {
        const auto& scope = tempStack.top();
        if (scope.find(name) != scope.end()) {
            return true;
        }
        tempStack.pop();
    }
    return false;
}