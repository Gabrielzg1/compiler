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

vector<string> SymbolTable::toPostFix(const vector<string>& input) {
    stack<string> operators;
    vector<string> output;

    // Função para determinar a precedência de um operador
    auto precedence = [](const string& op) -> int {
        if (op == "*" || op == "div") return 3;
        if (op == "+" || op == "-") return 2;
        if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=") return 1;
        if (op == "nao") return 4;
        if (op == "e") return 0;
        if (op == "ou") return -1;
        return -2;
    };

    // Função para verificar se o token é um operador
    auto isOperator = [](const string& token) -> bool {
        return token == "+" || token == "-" || token == "*" || token == "div" ||
               token == "==" || token == "!=" || token == "<" || token == ">" ||
               token == "<=" || token == ">=" || token == "nao" || token == "e" || token == "ou";
    };

    // Percorre a expressão infixa da esquerda para a direita
    for (const string& token : input) {
        if (!isOperator(token) && token != "(" && token != ")") {
            // Se for um operando (variável ou número), copia para a saída
            output.push_back(token);
        } else if (token == "(") {
            // Se for um abre-parênteses, empilha
            operators.push(token);
        } else if (token == ")") {
            // Se for um fecha-parênteses, desempilha até o abre-parênteses
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            // Remove o abre-parênteses da pilha
            if (!operators.empty()) {
                operators.pop();
            }
        } else if (isOperator(token)) {
            // Se for um operador, desempilha todos os operadores com precedência maior ou igual
            while (!operators.empty() && operators.top() != "(" && precedence(operators.top()) >= precedence(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            // Empilha o operador atual
            operators.push(token);
        }
    }

    // Ao terminar a expressão, desempilha todos os operadores restantes
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}


// Ver precedencia MAIOR OU IGUAL


