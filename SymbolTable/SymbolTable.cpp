#include "SymbolTable.h"

// Construtor da classe SymbolTable
SymbolTable::SymbolTable() : top(nullptr) {}

// Adiciona um novo símbolo no topo da pilha
void SymbolTable::push(SymbolInfo* symbolInfo) {
    Node* newNode = new Node();
    newNode->symbolInfo = symbolInfo;
    newNode->next = top;
    top = newNode;
}

// Remove o símbolo do topo da pilha
void SymbolTable::pop() {
    if (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

// Verifica se a pilha está vazia
bool SymbolTable::isEmpty() const {
    return top == nullptr;
}

// Retorna o símbolo do topo da pilha sem removê-lo
SymbolInfo* SymbolTable::peek() const {
    // Se não está vazia, retorna o símbolo do topo
    if (top != nullptr) {
        return top->symbolInfo;
    }
    return nullptr;
}