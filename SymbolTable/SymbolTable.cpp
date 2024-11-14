#include "SymbolTable.h"
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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
    while (aux != nullptr) {
        // Se encontrar o scopeLevel "L", faça a verificação e interrompa o loop
        if (aux->symbolInfo != nullptr && aux->symbolInfo->scopeLevel == "L") {
            // Coloque a verificação específica aqui, por exemplo, verificar se o nome é igual
            if (aux->symbolInfo->name == name) {
                return true;
            }
            break; // Interrompe o loop ao encontrar o "L"
        }

        // Continue verificando se o nome corresponde
        if (aux->symbolInfo != nullptr && aux->symbolInfo->name == name) {
            return true;
        }
        aux = aux->next;
    }
    return false;
}


bool SymbolTable::contains(std::string name) const {
    Node* aux = top;
    while (aux != nullptr && aux->symbolInfo != nullptr) {
        if (aux->symbolInfo->name == name) {
            return true;
        }
        aux = aux->next;
    }
    return false;
}



int SymbolTable::cutStack() {
    // Vai até encontrar o "L" e remove todos os elemento, assim que o encontra, retira o "L" do escopo
    int count = 0;
    while (top != nullptr && top->symbolInfo->scopeLevel != "L") {
        if(top->symbolInfo->type == "inteiro" || top->symbolInfo->type == "booleano"){
            count++;
        }
        pop();
    }
    if (top != nullptr) {
        top->symbolInfo->scopeLevel = "";
    }
    return count;
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


string SymbolTable::getType(const std::string& name) {
    Node *current = top;
    while (current != nullptr) {
        if (current->symbolInfo->name == name) {
            return current->symbolInfo->type;
        }
        current = current->next;
    }
    return "";
}

string SymbolTable::getAddress(const std::string& name) {
    Node *current = top;
    while (current != nullptr) {
        if (current->symbolInfo->name == name) {
            return current->symbolInfo->memoryAddress;
        }
        current = current->next;
    }
    return "";
}

bool SymbolTable::isProcedureOrProgram(const std::string& name) {
    Node *current = top;
    while (current != nullptr) {
        // Verifica se o identificador é um procedimento ou um programa
        if (current->symbolInfo->name == name &&
            (current->symbolInfo->type == "procedimento" || current->symbolInfo->type == "programa")) {
            return true;
        }
        current = current->next;
    }
    return false;
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
        if (op == "+u" || op == "-u") return 5;  // Precedência alta para operadores unários
        if (op == "*" || op == "div") return 4;  // Multiplicação e divisão
        if (op == "+" || op == "-") return 3;    // Adição e subtração binárias
        if (op == "=" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=") return 2;
        if (op == "nao") return 5;
        if (op == "e") return 1;
        if (op == "ou") return 0;
        return -1;
    };

    // Função para verificar se o token é um operador
    auto isOperator = [](const string& token) -> bool {
        return token == "+" || token == "-" || token == "*" || token == "div" ||
               token == "=" || token == "!=" || token == "<" || token == ">" ||
               token == "<=" || token == ">=" || token == "nao" || token == "e" || token == "ou" ||
               token == "+u" || token == "-u";  // Inclui operadores unários
    };

    for (size_t i = 0; i < input.size(); ++i) {
        const string& token = input[i];

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
            if (!operators.empty()) {
                operators.pop();  // Remove o abre-parênteses da pilha
            }
        } else if (isOperator(token)) {
            // Desempilha operadores com precedência maior ou igual antes de empilhar o atual
            while (!operators.empty() && operators.top() != "(" && precedence(operators.top()) >= precedence(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);  // Empilha o operador atual
        }
    }

    // Ao terminar a expressão, desempilha todos os operadores restantes
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}






