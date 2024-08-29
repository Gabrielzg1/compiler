#include "SymbolTable.h"

void SymbolTable::enterScope() {
    table.push(std::unordered_map<std::string, SymbolInfo>());
}

void SymbolTable::leaveScope() {
    if (!table.empty()) {
        table.pop();
    }
}

bool SymbolTable::addSymbol(const std::string& name, const std::string& type, int memoryAddress) {
    if (!table.empty()) {
        SymbolInfo symbolInfo = {name, static_cast<int>(table.size()), type, memoryAddress};
        table.top()[name] = symbolInfo;
        return true;
    }
    return false;
}

std::optional<SymbolInfo> SymbolTable::getSymbolInfo(const std::string& name) const {
    std::stack<std::unordered_map<std::string, SymbolInfo>> tempStack = table;
    while (!tempStack.empty()) {
        const auto& scope = tempStack.top();
        auto it = scope.find(name);
        if (it != scope.end()) {
            return it->second;
        }
        tempStack.pop();
    }
    return std::nullopt;
}

void SymbolTable::setTypeForVariables(const std::string& type) {
    if (!table.empty()) {
        for (auto& [name, symbolInfo] : table.top()) {
            if (symbolInfo.type == "variable") {
                symbolInfo.type = type;
            }
        }
    }
}

void SymbolTable::displayStack() const {
    std::stack<std::unordered_map<std::string, SymbolInfo>> tempStack = table;
    int scopeLevel = table.size();

    while (!tempStack.empty()) {
        const auto& scope = tempStack.top();
        std::cout << "Escopo " << scopeLevel-- << ":\n";
        for (const auto& [name, symbolInfo] : scope) {
            std::cout << "  Identificador: " << name << ", Tipo: " << symbolInfo.type << ", Endereco: " << symbolInfo.memoryAddress << "\n";
        }
        tempStack.pop();
    }
}