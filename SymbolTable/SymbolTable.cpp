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


string SymbolTable::inferType(const vector<string>& postFixExpr) {
    stack<string> typeStack;

    auto isOperator = [](const string& token) -> bool {
        return token == "+" || token == "-" || token == "*" || token == "div" ||
               token == "=" || token == "!=" || token == "<" || token == ">" ||
               token == "<=" || token == ">=" || token == "e" || token == "ou" || token == "nao" ||
               token == "+u" || token == "-u";
    };

    // Função para verificar se um token é um número
    auto isNumber = [](const string& token) -> bool {
        return !token.empty() && std::all_of(token.begin(), token.end(), ::isdigit);
    };

    for (const string& token : postFixExpr) {
        if (!isOperator(token)) {
            if (isNumber(token)) {
                // Se for um número, assume que é um inteiro
                typeStack.push("inteiro");
            } else if(token == "verdadeiro" || token == "falso") {
                typeStack.push("booleano");
            }
            else {
                // Caso contrário, deve ser um identificador e buscamos seu tipo
                string type = this->getType(token);
                if (type == "inteiro" || type == "booleano") {
                    typeStack.push(type);
                } else {
                    throw runtime_error("Erro: tipo invalido para o token '" + token + "'.");
                }
            }
        } else {
            if (token == "+" || token == "-" || token == "*" || token == "div") {
                if (typeStack.size() < 2) throw runtime_error("Erro: operandos insuficientes para operador '" + token + "'.");
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "inteiro" || left != "inteiro") {
                    throw runtime_error("Erro: operadores aritméticos '" + token + "' requerem inteiros.");
                }
                typeStack.push("inteiro");

            } else if (token == "+u" || token == "-u") {
                if (typeStack.empty()) throw runtime_error("Erro: operando insuficiente para operador unário '" + token + "'.");
                string operand = typeStack.top(); typeStack.pop();

                if (operand != "inteiro") {
                    throw runtime_error("Erro: operadores unários '" + token + "' requerem inteiros.");
                }
                typeStack.push("inteiro");

            } else if (token == "=" || token == "!=" || token == "<" || token == ">" ||
                       token == "<=" || token == ">=") {
                if (typeStack.size() < 2) throw runtime_error("Erro: operandos insuficientes para operador relacional '" + token + "'.");
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "inteiro" || left != "inteiro") {
                    throw runtime_error("Erro: operadores relacionais '" + token + "' requerem inteiros.");
                }
                typeStack.push("booleano");

            } else if (token == "e" || token == "ou") {
                if (typeStack.size() < 2) throw runtime_error("Erro: operandos insuficientes para operador lógico '" + token + "'.");
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "booleano" || left != "booleano") {
                    throw runtime_error("Erro: operadores lógicos '" + token + "' requerem booleanos.");
                }
                typeStack.push("booleano");

            } else if (token == "nao") {
                if (typeStack.empty()) throw runtime_error("Erro: operando insuficiente para operador 'nao'.");
                string operand = typeStack.top(); typeStack.pop();

                if (operand != "booleano") {
                    throw runtime_error("Erro: operador 'nao' requer booleano.");
                }
                typeStack.push("booleano");
            }
        }
    }

    if (typeStack.size() != 1) throw runtime_error("Erro: expressão malformada.");

    return typeStack.top();
}




