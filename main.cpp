#include <iostream>
#include "Lexical/Lexical.h"
#include "Token/Token.h"
using namespace std;

// Variáveis Globais
Lexical lexer("code.txt");
SymbolTable* symboltable = new SymbolTable();
Token token = lexer.getNextToken();

void getNextToken();
void analysisFunction();
void analysisProcedure();
void analysisSubroutine();
void typeAnalysis();
void variablesAnalysis();
void variablesDeclarationAnalysis();
void blockAnalysis();
void commandsAnalysis();
void simpleCommand();
void atrib_chproc();
void ifAnalysis();
void whileAnalysis();
void readAnalysis();
void writeAnalysis();
void atribAnalysis();
void functionCallAnalysis();
void expressionAnalysis();
void termAnalysis();
void factorAnalysis();
void procedureCallAnalysis();
void simpleExpressionAnalysis();

void getNextToken() {
    cout << token.getTypeString() << endl;
    token = lexer.getNextToken();

}



void simpleExpressionAnalysis(){
    if(token.getTypeString() == "smais" || token.getTypeString() == "smenos"){
        getNextToken();
    }
    termAnalysis();
    while(token.getTypeString() == "smais" || token.getTypeString() == "smenos" || token.getTypeString() == "sou"){
        getNextToken();
        termAnalysis();
    }
}

void functionCallAnalysis() {
    getNextToken();
}
void procedureCallAnalysis(){
    //getNextToken();
}


void atribAnalysis(){
    getNextToken();
    expressionAnalysis();
}


void factorAnalysis() {
    if(token.getTypeString() == "sidentificador") {
        functionCallAnalysis();
    } else if (token.getTypeString() == "snumero") {
        getNextToken();
    } else if (token.getTypeString() == "snao") {
        getNextToken();
        factorAnalysis();
    } else if (token.getTypeString() == "sabre_parenteses") {
        getNextToken();
        expressionAnalysis();
        if(token.getTypeString() == "sfecha_parenteses") {
            getNextToken();
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se ')' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else if (token.getTypeString() == "sverdadeiro" || token.getTypeString() == "sfalso") {
        getNextToken();
    }
    else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador', 'numero', 'nao' ou '(' na linha: " + std::to_string(lexer.getCurrentLine()));
    }

}

void termAnalysis() {
    factorAnalysis();
    while(token.getTypeString() == "smult" || token.getTypeString() == "sdiv" || token.getTypeString() == "se") {
        getNextToken();
        factorAnalysis();
    }
}

void expressionAnalysis() {
    simpleExpressionAnalysis();
    if(token.getTypeString() == "smaior" || token.getTypeString() == "smaiorig" ||
        token.getTypeString() == "sig" || token.getTypeString() == "smenor" ||
        token.getTypeString() == "smenorig" || token.getTypeString() == "sdif") {
        getNextToken();
        simpleExpressionAnalysis();
    }


}
void readAnalysis(){
    getNextToken();
    if(token.getTypeString() == "sabre_parenteses"){
        getNextToken();
        if(token.getTypeString() == "sidentificador"){
            getNextToken();
            if(token.getTypeString() == "sfecha_parenteses"){
                getNextToken();
            } else {
                throw std::runtime_error("Erro de Sintaxe! Espera-se ')' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
    } else
        throw std::runtime_error("Erro de Sintaxe! Espera-se '(' na linha: " + std::to_string(lexer.getCurrentLine()));
}
void writeAnalysis(){
    getNextToken();
    if(token.getTypeString() == "sabre_parenteses"){
        getNextToken();
        if(token.getTypeString() == "sidentificador"){
            getNextToken();
            if(token.getTypeString() == "sfecha_parenteses"){
                getNextToken();
            } else {
                throw std::runtime_error("Erro de Sintaxe! Espera-se ')' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
    } else
        throw std::runtime_error("Erro de Sintaxe! Espera-se '(' na linha: " + std::to_string(lexer.getCurrentLine()));
}

void atrib_chproc() {
    getNextToken();

    if(token.getTypeString() == "satribuicao")
        atribAnalysis();
    else
        procedureCallAnalysis();
}

void ifAnalysis(){
    getNextToken();

    expressionAnalysis();
    if(token.getTypeString() == "sentao"){

        getNextToken();
        simpleCommand();
        if(token.getTypeString() == "ssenao"){
            getNextToken();
            simpleCommand();
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'entao' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}


void whileAnalysis(){
    getNextToken();
    expressionAnalysis();
    if(token.getTypeString() == "sfaca"){
        getNextToken();
        simpleCommand();
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'faca' na linha: " + std::to_string(lexer.getCurrentLine()));
    }

}




void simpleCommand(){
    if(token.getTypeString() == "sidentificador"){
        atrib_chproc();
    } else if (token.getTypeString() == "sse"){
        ifAnalysis();
    } else if (token.getTypeString() == "senquanto"){
        whileAnalysis();
    } else if (token.getTypeString() == "sleia") {
        readAnalysis();
    } else if (token.getTypeString() == "sescreva") {
        writeAnalysis();
    } else {
       commandsAnalysis();
    }

}

void commandsAnalysis() {
    if (token.getTypeString() == "sinicio") {
        getNextToken();
        simpleCommand();
        while (token.getTypeString() != "sfim") {
            if (token.getTypeString() == "sponto_virgula") {
                getNextToken();
                if (token.getTypeString() != "sfim") {
                    simpleCommand();
                }
            } else {
                cout << 1 << endl;
                throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        }
        getNextToken();
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'inicio' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}


void analysisFunction() {
    getNextToken();
    if (token.getTypeString() == "sidentificador") {
        getNextToken();
        if(token.getTypeString() == "sdoispontos"){
            getNextToken();
            if(token.getTypeString() == "sinteiro" || token.getTypeString() == "sbooleano"){
                getNextToken();
                if(token.getTypeString() == "sponto_virgula"){
                    blockAnalysis();
                } else {
                    cout << 2 << endl;
                    throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            } else {
                throw std::runtime_error("Erro de Sintaxe! Tipo invalido na linha: " + std::to_string(lexer.getCurrentLine()));
            }
            if(token.getTypeString() == "sponto_virgula"){
                blockAnalysis();
            } else {
                cout << 3 << endl;
                throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se ':' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}

void analysisProcedure() {
    getNextToken();
    if (token.getTypeString() == "sidentificador") {
        getNextToken();
        if(token.getTypeString() == "sponto_virgula"){
            blockAnalysis();
        } else {
            cout << 4 << endl;
            throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}


void analysisSubroutine() {
    while (token.getTypeString() == "sprocedimento" || token.getTypeString() == "sfuncao") {
        if (token.getTypeString() == "sprocedimento") {
            analysisProcedure();
        } else {
            analysisFunction();
        }
        // Rever esse trecho com o freitas

        /*   if (token.getTypeString() == "sponto_virgula") {
            getNextToken();
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
*/
    }
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
                    cout << 6 << endl;
                    throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    }
}

void blockAnalysis() {
    getNextToken();
    // Analisa declarações de variáveis
    variablesDeclarationAnalysis();
    // Analisa subrotinas
    analysisSubroutine();
    // Analisa comandos
    commandsAnalysis();

}

int main() {
    if (token.getTypeString() == "sprograma"){
        getNextToken();
        if(token.getTypeString() == "sidentificador"){
            symboltable->push(token.getLexeme(), 0, "programa", 0);
            getNextToken();
            if(token.getTypeString() == "sponto_virgula"){
                blockAnalysis();
                 if(token.getTypeString() == "sponto"){
                     getNextToken();
                     if(token.getTypeString() == "endfile"){
                        cout << endl << " ------ Compilado com sucesso! --------" << endl << endl;
                     } else
                        throw std::runtime_error("ERRO sintatico: " + std::to_string(lexer.getCurrentLine()));
                } else {
                    throw std::runtime_error("Sintax Error! Espera-se '.' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            } else {
                cout << 7 << endl;
                throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'programa' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
    cout << "Pilha de simbolos: " << endl;
    symboltable->printStack();
    return 0;
}
