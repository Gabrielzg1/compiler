#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stack>
#include <unordered_map>
#include <string>

class SymbolTable {
public:
    void enterScope();
    void leaveScope();
    bool addSymbol(const std::string& name);
    bool isSymbolVisible(const std::string& name) const;

private:
    std::stack<std::unordered_map<std::string, bool>> table;
};

#endif // SYMBOLTABLE_H
