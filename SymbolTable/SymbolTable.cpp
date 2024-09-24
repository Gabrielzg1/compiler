#include "SymbolTable.h"
#include <iostream>

SymbolTable::SymbolTable() : top(nullptr) {}

SymbolTable::~SymbolTable() {
    while (!isEmpty()) {
        pop();
    }
}

void SymbolTable::push(string name, int scopeLevel, string type, int memoryAddress) {
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
        delete temp->symbolInfo;
        delete temp;
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

bool SymbolTable::contains(std::string name) {
    Node* aux = top;
    while (aux != nullptr) {
        if (aux->symbolInfo->name == name) {
            return true;
        }
        aux = aux->next;
    }
    return false;
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