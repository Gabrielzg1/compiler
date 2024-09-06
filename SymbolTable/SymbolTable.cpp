#include "SymbolTable.h"

// Construtor da classe SymbolTableTest
SymbolTable::SymbolTable() : top(nullptr) {}

// Adiciona um novo símbolo no topo da pilha
void SymbolTable::push( string name, int scopeLevel, string type, int memoryAddress) {
    SymbolInfo* symbolInfo = new SymbolInfo();
    symbolInfo->name = name;
    symbolInfo->scopeLevel = scopeLevel;
    symbolInfo->type = type;
    symbolInfo->memoryAddress = memoryAddress;

    // Criando o Node com as informacoes do simbolo
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
void SymbolTable::assignTypeToVariables(const std::string& newType) {
    Node* current = top;
    while (current != nullptr) {
        if (current->symbolInfo->type == "var") {
            current->symbolInfo->type = newType;
        }
        current = current->next;
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

bool SymbolTable::contains(std::string name){
    Node* aux = top;
    while(aux != nullptr){
        if(aux->symbolInfo->name == name){
            return true;
        }
        aux = aux->next;
    }
    return false;
}