#include <iostream>
#include "Lexical/Lexical.h"
#include "Token/Token.h"
using namespace std;

// Variáveis Globais
Lexical lexer("code.txt");
SymbolTable* symboltable = new SymbolTable();
Token token = lexer.getNextToken();

void getNextToken() {
    token = lexer.getNextToken();
}

void typeAnalysis(){
    if(token.getTypeString() != "sinteiro" && token.getTypeString() != "sbooleano"){
        throw std::runtime_error("Erro de Sintaxe! Tipo invalido na linha: " + std::to_string(lexer.getCurrentLine()));
    } else
        symboltable->assignTypeToVariables(token.getLexeme());
    getNextToken();
}

void variablesAnalysis() {
    do {
        if (token.getTypeString() == "sidentificador") {
            if (!symboltable->contains(token.getLexeme())) {
                symboltable->push(token.getLexeme(), 0, "var", 0);
                getNextToken();
                if (token.getTypeString() == "svirgula" || token.getTypeString() == "sdoispontos") {
                    if (token.getTypeString() == "svirgula") {
                        getNextToken();
                        if (token.getTypeString() == "sdoispontos") {
                            throw std::runtime_error("Erro de Sintaxe! ':' inesperado na linha: " +
                                                     std::to_string(lexer.getCurrentLine()));
                        }
                    } else if (token.getTypeString() == "sdoispontos") {
                        break;
                    }
                } else {
                    throw std::runtime_error("Erro de Sintaxe! Esperava-se ',' ou ':' na linha: " +
                                             std::to_string(lexer.getCurrentLine()));
                }
            } else {
                throw std::runtime_error("Erro de Sintaxe! Variavel ja declarada na linha: " +
                                         std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Esperava-se 'identificador' na linha: " +
                                     std::to_string(lexer.getCurrentLine()));
        }

    } while (token.getTypeString() != "sdoispontos");

    getNextToken();
    typeAnalysis();
}



void variablesDeclarationAnalysis() {
    getNextToken();
    if(token.getTypeString() == "svar"){
        getNextToken();
        if (token.getTypeString() == "sidentificador") {
            while(token.getTypeString() == "sidentificador"){
                // Analise de variaveis
                variablesAnalysis();
                if(token.getTypeString() == "sponto_virgula"){
                    getNextToken(); // Avança o token após o ponto e vírgula
                    continue;
                } else {
                    throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'var' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}

void blockAnalysis() {
    // Analisa declarações de variáveis
    variablesDeclarationAnalysis();
    // Analisa subrotinas (não implementado aqui)

    // Analisa comandos (não implementado aqui)
}

int main() {
    if (token.getTypeString() == "sprograma"){
        getNextToken();
        if(token.getTypeString() == "sidentificador"){
            symboltable->push(token.getLexeme(), 0, "programa", 0);
            getNextToken();
            if(token.getTypeString() == "sponto_virgula"){
                blockAnalysis();
                /* getNextToken();
                 if(token.getTypeString() == "sponto"){
                    getNextToken();
                    if(token.getTypeString() == "TOKEN_UNKNOWN"){
                        cout << "Compilado com sucesso!" << endl;
                    } else
                        throw std::runtime_error("ERROR, ERROR, ERROR" + std::to_string(lexer.getCurrentLine()));
                } else {
                    throw std::runtime_error("Sintax Error! Espera-se '.' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
                 */
                cout << "Compilado com sucesso!" << endl;
            } else {
                throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'programa' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
    symboltable->printStack();
    return 0;
}
