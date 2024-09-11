#include <iostream>
#include "Lexical/Lexical.h"
#include "Token/Token.h"
using namespace std;

// Variável global para o analisador léxico
Lexical lexer("code.txt");
SymbolTable* symboltable = new SymbolTable();


bool variablesAnalysis(){
    return true;
}


bool variablesDeclarationAnalysis() {
    if(lexer.getNextToken().getTypeString() == "svar"){
        while(lexer.getNextToken().getTypeString() == "sidentificador"){
            // Analise de variaveis

        }
        return true;
    } else {
        throw std::runtime_error("Sintax Error! Espera-se 'var' na linha: " + std::to_string(lexer.getCurrentLine()));

    }

}


bool blockAnalysis() {
    // Analisa declarações de variáveis
    variablesDeclarationAnalysis();
    // Analisa Subrotinas

    // Analisa Comandos

    return true;
}

int main() {
    if (lexer.getNextToken().getTypeString() == "sprograma"){
        if(lexer.getNextToken().getTypeString() == "sidentificador"){
            if(lexer.getNextToken().getTypeString() == "sponto_virgula"){
                cout << "Programa reconhecido!" << endl;
                blockAnalysis();
                // Analisa Bloco a ser implmentado
            } else {
                throw std::runtime_error("Sintax Error! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Sintax Error! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
            throw std::runtime_error("Sintax Error! Espera-se 'programa' na linha: " + std::to_string(lexer.getCurrentLine()));
    }

    return 0;
}

