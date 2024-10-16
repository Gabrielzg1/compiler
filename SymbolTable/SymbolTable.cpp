#include "SymbolTable.h"
#include <stack>
#include <vector>
#include <string>
#include <iostream>

SymbolTable::SymbolTable() : top(nullptr) {}

SymbolTable::~SymbolTable() {
    while (!isEmpty()) {
        pop();
    }
}

void SymbolTable::push(string name, string scopeLevel, string type, string memoryAddress) {
    SymbolInfo* symbolInfo = new SymbolInfo();
    symbolInfo->name = name;
    symbolInfo->scopeLevel = scopeLevel;
    symbolInfo->type = type;
    symbolInfo->memoryAddress = memoryAddress;

    Node* newNode = new Node();
    newNode->symbolInfo = symbolInfo;
    newNode->next = top;
    top = newNode;
}

void SymbolTable::pop() {
    if (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp->symbolInfo; // Liberar a memória corretamente
        delete temp;  // Não deixar vazamento de memória
    }
}


bool SymbolTable::isEmpty() const {
    return top == nullptr;
}

SymbolInfo* SymbolTable::peek() const {
    if (top != nullptr) {
        return top->symbolInfo;
    }
    return nullptr;
}

bool SymbolTable::containsVar(std::string name) const {
    Node* aux = top;
    while (aux != nullptr && aux->symbolInfo != nullptr && aux->symbolInfo->scopeLevel != "L") {
        if (aux->symbolInfo->name == name) {
            return true;
        }
        aux = aux->next;
    }
    return false;
}

bool SymbolTable::containsProcFunc(std::string name) const {
    Node* aux = top;
    while (aux != nullptr && aux->symbolInfo != nullptr) {
        if (aux->symbolInfo->name == name) {
            return true;
        }
        aux = aux->next;
    }
    return false;
}

void SymbolTable::cutStack() {
    // Vai até encontrar o "L" e remove todos os elemento, assim que o encontra, retira o "L" do escopo
    while (top != nullptr && top->symbolInfo->scopeLevel != "L") {
        pop();
    }
    if (top != nullptr) {
        top->symbolInfo->scopeLevel = "";
    }
}



void SymbolTable::assignTypeToVariables(const std::string& newType) {
    Node* current = top;
    while (current != nullptr) {
        if (current->symbolInfo->type == "var") {
            current->symbolInfo->type = newType;
        }
        current = current->next;
    }
}

void SymbolTable::assignTypeToFunction(const std::string& newType) {
    Node* current = top;
    while (current != nullptr) {
        if (current->symbolInfo->type == "function") {
            current->symbolInfo->type = newType;
        }
        current = current->next;
    }
}

string SymbolTable::getFuncType(const std::string& name) {
    Node *current = top;
    while (current != nullptr) {
        if (current->symbolInfo->name == name) {
            return current->symbolInfo->type;
        }
        current = current->next;
    }
    return "";
}

 void SymbolTable::printStack() const {
    Node* current = top;
    while (current != nullptr) {
        std::cout << "Name: " << current->symbolInfo->name
                  << ", Scope Level: " << current->symbolInfo->scopeLevel
                  << ", Type: " << current->symbolInfo->type
                  << ", Memory Address: " << current->symbolInfo->memoryAddress
                  << std::endl;
        current = current->next;
    }
}


