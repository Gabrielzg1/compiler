#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
#include <vector>
#include <algorithm>
#include "Lexical/Lexical.h"
#include "Token/Token.h"
using namespace std;

// Variáveis Globais
Lexical lexer("code.txt");
SymbolTable* symboltable = new SymbolTable();
Token token = lexer.getNextToken();
int label = 1;
int memoryPosition = 1;

// Arquivo de saída para salvar os resultados
ofstream outputFile("output.txt");
ofstream assemblyFile("assembly.txt");

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
void atribAnalysis(const string& type);
void functionCallAnalysis();
void expressionAnalysis(std::vector<std::string>& infixExpression);
void termAnalysis(std::vector<std::string>& infixExpression);
void factorAnalysis(std::vector<std::string>& infixExpression);
void procedureCallAnalysis();
void simpleExpressionAnalysis(std::vector<std::string>& infixExpression);

void getNextToken() {
    //cout << token.getTypeString() + " -> " + token.getLexeme() << endl;
    token = lexer.getNextToken();
}

void gera(string label, string instruction, string attribute1, string attribute2){
    assemblyFile << label << " " << instruction << " " << attribute1 << " " << attribute2 << endl;
}

void geraExpressao(const vector<string>& postfix) {
    for (const auto& i : postfix) {
        if (i == "nao") {
            gera(" ", "NEG", "", "");
        } else if (i == "e") {
            gera(" ", "AND", "", "");
        } else if (i == "ou") {
            gera(" ", "OR", "", "");
        } else if (i == "+") {
            gera(" ", "ADD", "", "");
        } else if (i == "-") {
            gera(" ", "SUB", "", "");
        } else if (i == "*") {
            gera(" ", "MULT", "", "");
        } else if (i == "div") {
            gera(" ", "DIVI", "", ""); // Corrigido para 'DIVI' conforme a instrução
        } else if (i == "=") {
            gera(" ", "CEQ", "", "");
        } else if (i == "!=") {
            gera(" ", "CDIF", "", "");
        } else if (i == "<") {
            gera(" ", "CME", "", "");
        } else if (i == ">") {
            gera(" ", "CMA", "", "");
        } else if (i == "<=") {
            gera(" ", "CMEQ", "", "");
        } else if (i == ">=") {
            gera(" ", "CMAQ", "", "");
        } else if (i == "+u") {
            gera(" ", "INV", "", "");  // Para inversão de sinal positivo
        } else if (i == "-u") {
            gera(" ", "NEG", "", "");  // Para inversão de sinal negativo
        } else if (i == "verdadeiro") {
            gera(" ", "LDC", "1", ""); // Carregar constante '1' para verdadeiro
        } else if (i == "falso") {
            gera(" ", "LDC", "0", ""); // Carregar constante '0' para falso
        } else if (symboltable->contains(i)) {
            gera(" ", "LDV", symboltable->getAddress(i), ""); // Carregar valor de variável
        } else {
            gera(" ", "LDC", i, "");  // Carregar constante genérica
        }
    }
}


string inferType(const vector<string>& postFixExpr) {
    stack<string> typeStack;

    // Função auxiliar para verificar se o token é um operador
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

    // Processa a expressão pós-fixada
    for (const string& expr : postFixExpr) {
        if (!isOperator(expr)) {
            if (isNumber(expr)) {
                // Se for um número, assume que é um inteiro
                typeStack.push("inteiro");
            } else {
                // Se for um identificador, busca o tipo na tabela de símbolos
                if(expr == "verdadeiro" || expr == "falso") {
                    typeStack.push("booleano");
                } else {
                    string type = symboltable->getType(expr);
                    if (type == "inteiro" || type == "booleano") {
                        typeStack.push(type);
                    } else if (type == "funcao inteiro" ) {
                        typeStack.push("inteiro");
                    } else if (type == "funcao booleano"){
                        typeStack.push("booleano");
                    }
                    else {
                        throw runtime_error("Tipo invalido para o token '" + expr + "'." + " Linha: " + std::to_string(lexer.getCurrentLine()));
                    }
                }
            }
        } else {
            if (expr == "+" || expr == "-" || expr == "*" || expr == "div") {
                // Operadores aritméticos binários: ambos operandos devem ser inteiros
                if (typeStack.size() < 2) throw runtime_error("Operandos insuficientes.");
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "inteiro" || left != "inteiro") {
                    throw runtime_error("Operadores aritmeticos requerem inteiros. linha: " + std::to_string(lexer.getCurrentLine()));
                }

                // Resultado também é inteiro
                typeStack.push("inteiro");

            } else if (expr == "+u" || expr == "-u") {
                // Operadores aritméticos unários: operando deve ser inteiro
                if (typeStack.empty()) throw runtime_error("Operandos insuficientes.");
                string operand = typeStack.top(); typeStack.pop();

                if (operand != "inteiro") {
                    throw runtime_error("Operadores unarios requerem inteiros. Linha: " + std::to_string(lexer.getCurrentLine()));
                }

                // Resultado também é inteiro
                typeStack.push("inteiro");

            } else if (expr == "=" || expr == "!=") {
                // Operadores de igualdade: ambos operandos devem ser do mesmo tipo
                if (typeStack.size() < 2) throw runtime_error("Operandos insuficientes. Linha: " + std::to_string(lexer.getCurrentLine()));
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != left) {
                    throw runtime_error("Operadores de igualdade requerem operandos do mesmo tipo. Linha: " + std::to_string(lexer.getCurrentLine()));
                }

                // Resultado é booleano
                typeStack.push("booleano");

            } else if (expr == "<" || expr == ">" || expr == "<=" || expr == ">=") {
                // Operadores relacionais: ambos operandos devem ser inteiros
                if (typeStack.size() < 2) throw runtime_error("Operandos insuficientes. Linha: " + std::to_string(lexer.getCurrentLine()));
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "inteiro" || left != "inteiro") {
                    throw runtime_error("Operadores relacionais requerem inteiros. Linha: " + std::to_string(lexer.getCurrentLine()));
                }

                // Resultado é booleano
                typeStack.push("booleano");

            } else if (expr == "e" || expr == "ou") {
                // Operadores lógicos binários: ambos operandos devem ser booleanos
                if (typeStack.size() < 2) throw runtime_error("Operandos insuficientes. Linha: " + std::to_string(lexer.getCurrentLine()));
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "booleano" || left != "booleano") {
                    throw runtime_error("Operadores logicos requerem booleanos. Linha: " + std::to_string(lexer.getCurrentLine()));
                }

                // Resultado também é booleano
                typeStack.push("booleano");

            } else if (expr == "nao") {
                // Operador unário lógico: operando deve ser booleano
                if (typeStack.empty()) throw runtime_error("Operandos insuficientes. Linha: " + std::to_string(lexer.getCurrentLine()));
                string operand = typeStack.top(); typeStack.pop();

                if (operand != "booleano") {
                    throw runtime_error("Operador 'nao' requer booleano. Linha: " + std::to_string(lexer.getCurrentLine()));
                }

                // Resultado também é booleano
                typeStack.push("booleano");
            }
        }
    }

    // Ao final, deve sobrar apenas um tipo na pilha
    if (typeStack.size() != 1) throw runtime_error("Erro: expressão malformada. Linha: " + std::to_string(lexer.getCurrentLine()));

    // Retorna o tipo final
    return typeStack.top();
}



/*
 *
 *  Sintatico ----------------------------------------------
 *
 *
 *
 *
 *
 */


void simpleExpressionAnalysis(std::vector<std::string>& infixExpression) {
    if (token.getTypeString() == "smais" || token.getTypeString() == "smenos") {
        // Adicionar operador unário ao vetor de expressão infix
        if(token.getTypeString() == "smais")
            infixExpression.push_back("+u");
        else
            infixExpression.push_back("-u");
        getNextToken();
    }
    termAnalysis(infixExpression);
    while (token.getTypeString() == "smais" || token.getTypeString() == "smenos" || token.getTypeString() == "sou") {
        // Adicionar operador ao vetor de expressão infix
        infixExpression.push_back(token.getLexeme());
        getNextToken();
        termAnalysis(infixExpression);
    }
}

void functionCallAnalysis() {
    // Alterar na geracao de código
    //implementar a busca pelo rotulo na tabela de simbolos e gerar o código
    string address = symboltable->getAddress(token.getLexeme());
    gera(" ", "CALL", std::move(address), "");
    gera(" ", "LDV", "0", "");
    getNextToken();
}

void procedureCallAnalysis(string address) {
    gera(" ", "CALL", std::move(address), "");
    // Geracao de código
}


// Colocar o STR AQUI
void atribAnalysis(const string& type) {

    getNextToken();
    // Criar e passar por referência o vetor de strings para a expressão infix
    std::vector<std::string> infixExpression;

    expressionAnalysis(infixExpression);
    vector<string> postfix = symboltable->toPostFix(infixExpression);
    string expressionType = inferType(postfix);
    if(expressionType != type){
        throw std::runtime_error("Atribuicao de tipos diferentes na linha: " + std::to_string(lexer.getCurrentLine()));
    }
    geraExpressao(postfix);


}

void factorAnalysis(std::vector<std::string>& infixExpression) {
    if (token.getTypeString() == "sidentificador") {
        if (symboltable->contains(token.getLexeme())) {
            if (symboltable->getType(token.getLexeme()) == "funcao inteiro" || symboltable->getType(token.getLexeme()) == "funcao booleano") {
                infixExpression.push_back(token.getLexeme());
                functionCallAnalysis();
            } else {
                if (!symboltable->isProcedureOrProgram(token.getLexeme())) {
                    // Adicionar variável ao vetor de expressão infix
                    infixExpression.push_back(token.getLexeme());
                    getNextToken();


                } else {
                    throw std::runtime_error("Procedimento usado indevidamente na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            }
        } else {
            throw std::runtime_error("Variável não declarada na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else if (token.getTypeString() == "snumero") {
        // Adicionar número ao vetor de expressão infix
        infixExpression.push_back(token.getLexeme());
        getNextToken();
    } else if (token.getTypeString() == "snao") {
        getNextToken();
        infixExpression.push_back("nao");
        factorAnalysis(infixExpression);
    } else if (token.getTypeString() == "sabre_parenteses") {
        getNextToken();
        infixExpression.push_back("(");
        expressionAnalysis(infixExpression);
        if (token.getTypeString() == "sfecha_parenteses") {
            infixExpression.push_back(")");
            getNextToken();
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se ')' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else if (token.getTypeString() == "sverdadeiro" || token.getTypeString() == "sfalso") {
        // Adicionar verdadeiro ou falso ao vetor de expressão infix
        infixExpression.push_back(token.getLexeme());
        getNextToken();
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador', 'numero', 'nao' ou '(' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}

void termAnalysis(std::vector<std::string>& infixExpression) {
    factorAnalysis(infixExpression);
    while (token.getTypeString() == "smult" || token.getTypeString() == "sdiv" || token.getTypeString() == "se") {
        // Adicionar operador ao vetor de expressão infix
        infixExpression.push_back(token.getLexeme());
        getNextToken();
        factorAnalysis(infixExpression);
    }
}

void expressionAnalysis(std::vector<std::string>& infixExpression) {
    simpleExpressionAnalysis(infixExpression);
    if (token.getTypeString() == "smaior" || token.getTypeString() == "smaiorig" ||
        token.getTypeString() == "sig" || token.getTypeString() == "smenor" ||
        token.getTypeString() == "smenorig" || token.getTypeString() == "sdif") {
        // Adicionar operador relacional ao vetor de expressão infix
        infixExpression.push_back(token.getLexeme());
        getNextToken();
        simpleExpressionAnalysis(infixExpression);
    }
}

void readAnalysis(){
    getNextToken();
    if(token.getTypeString() == "sabre_parenteses"){
        getNextToken();
        if(token.getTypeString() == "sidentificador"){
            if(symboltable->contains(token.getLexeme()) && symboltable->getType(token.getLexeme()) == "inteiro"){
                /* Inserir Lógica de busca geracao de código -> pega a primeira ocorrencia da variavel
                 *
                 * /////
                 *
                 *
                */
                gera(" ", "RD", "", "");
                gera(" ", "STR", symboltable->getAddress(token.getLexeme()), "");

                getNextToken();
                if(token.getTypeString() == "sfecha_parenteses"){
                    getNextToken();
                } else {
                    throw std::runtime_error("Erro de Sintaxe! Espera-se ')' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            }
            else {
                throw std::runtime_error("Variavel nao declarada ou tipo incompativel na linha: " + std::to_string(lexer.getCurrentLine()));
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
            if(symboltable->contains(token.getLexeme()) && symboltable->getType(token.getLexeme()) == "inteiro"){
                /* Inserir Lógica de busca geracao de código -> pega a primeira ocorrencia da variavel
                 *
                 * /////
                 *
                */
                gera(" ", "LDV", symboltable->getAddress(token.getLexeme()), "");
                gera(" ", "PRN", "", "");



                getNextToken();
                if(token.getTypeString() == "sfecha_parenteses"){
                    getNextToken();
                } else {
                    throw std::runtime_error("Erro de Sintaxe! Espera-se ')' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            } else {
                throw std::runtime_error("Variavel nao declarada ou tipo incompativel na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
    } else
        throw std::runtime_error("Erro de Sintaxe! Espera-se '(' na linha: " + std::to_string(lexer.getCurrentLine()));
}

void atrib_chproc() {
    // Verificar se a variável está declarada na tabela de símbolos
    int flag = 0;
    if (symboltable->contains(token.getLexeme())) {
        string type = symboltable->getType(token.getLexeme());
        string address = symboltable->getAddress(token.getLexeme());

        getNextToken();
        if(type == "funcao inteiro"){
            type = "inteiro";
            flag = 1;

        }
        else if(type == "funcao booleano"){
            type = "booleano";
            flag = 1;
        }
        // Verificar se é uma atribuição ou chamada de procedimento com base no tipo e token atual
        if ((token.getTypeString() == "satribuicao") && (type == "inteiro" || type == "booleano" || type == "funcao inteiro" || type == "funcao booleano")) {
            atribAnalysis(type);

            if(type == "inteiro" || type == "booleano"){
                if(flag == 1){
                    gera(" ", "STR", "0", "");
                }
                else
                    gera(" ", "STR", address, "");
            }
        } else if (type == "procedimento") {
            procedureCallAnalysis(address);
        } else {
            // Tipo inválido para atribuição ou chamada de procedimento
            throw std::runtime_error("Tipo invalido na atribuição/chamada de procedimento na linha " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        // Variável não declarada
        throw std::runtime_error("Variavel nao declarada na linha " + std::to_string(lexer.getCurrentLine()));
    }
}


void ifAnalysis() {
    getNextToken();
    int auxLabel1, auxLabel2;

    // Análise da expressão condicional E
    std::vector<std::string> infixExpression;
    expressionAnalysis(infixExpression);
    vector<string> postfix = symboltable->toPostFix(infixExpression);

    geraExpressao(postfix);
    for(auto i : postfix){
        cout << i << " ";
    }
    cout << endl;

    string expressionType = inferType(postfix);

    if (expressionType != "booleano") {
        throw std::runtime_error("Atribuicao de tipos diferentes na linha: " + std::to_string(lexer.getCurrentLine()));
    }

    // Rótulo para pular caso a expressão E seja falsa
    auxLabel1 = label;
    gera(" ", "JMPF", to_string(auxLabel1), "");
    label++;

    // Analisar o bloco C1 (`entao`)
    if (token.getTypeString() == "sentao") {
        getNextToken();
        simpleCommand();

        // Se houver `senao`
        if (token.getTypeString() == "ssenao") {
            auxLabel2 = label;

            // Gerar JMP para pular o bloco `senao` após `entao`
            gera(" ", "JMP", to_string(auxLabel2), "");

            // Marcar o início do bloco `senao`
            gera(to_string(auxLabel1), "NULL", "", "");

            getNextToken();
            simpleCommand();

            // Finalizar o bloco `senao`
            gera(to_string(auxLabel2), "NULL", "", "");
        } else {
            // Caso não haja `senao`, finalizar o bloco `entao`
            gera(to_string(auxLabel1), "NULL", "", "");
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'entao' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}

void whileAnalysis() {
    getNextToken();
    int auxLabel1, auxLabel2;
    auxLabel1 = label;
    gera(to_string(label), "NULL", "", "");
    label++;
    // Criar e passar por referência o vetor de strings para a expressão infix
    std::vector<std::string> infixExpression;
    expressionAnalysis(infixExpression);
    vector<string> postfix = symboltable->toPostFix(infixExpression);
    string expressionType = inferType(postfix);


    if(expressionType != "booleano"){
        throw std::runtime_error("Atribuicao de tipos diferentes na linha: " + std::to_string(lexer.getCurrentLine()));
    }
    // Colocar aqui geracao de código
    geraExpressao(postfix);

    if (token.getTypeString() == "sfaca") {
        auxLabel2 = label;

        gera(" ", "JMPF", to_string(label), "");
        getNextToken();
        simpleCommand();

        gera(" ", "JMP", to_string(auxLabel1), "");
        gera(to_string(auxLabel2), "NULL", "", "");
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'faca' ou um operador logico na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}

void simpleCommand(){
    if(token.getTypeString() == "sidentificador"){
        // ponto de atencao
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
                throw std::runtime_error("Erro de Sintaxe! Padrao invalido na linha: " + std::to_string(lexer.getCurrentLine()));
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
        if(!symboltable->contains(token.getLexeme())){
            symboltable->push(token.getLexeme(), "L", "function", to_string(label));

            //Gera a marcação da função e soma 1 ao label
            gera(to_string(label), "NULL", "", "");
            label++;

            getNextToken();
            if(token.getTypeString() == "sdoispontos"){
                getNextToken();
                if(token.getTypeString() == "sinteiro" || token.getTypeString() == "sbooleano"){
                    symboltable->assignTypeToFunction("funcao " + token.getLexeme());
                    getNextToken();
                    if(token.getTypeString() == "sponto_virgula"){
                        blockAnalysis();
                    }
                } else {
                    throw std::runtime_error("Erro de Sintaxe! Tipo invalido na linha: " + std::to_string(lexer.getCurrentLine()));
                }

            } else {
                throw std::runtime_error("Erro de Sintaxe! Espera-se ':' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Funcao ja declarada na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
    int count = symboltable->cutStack();
    if(count > 0) {
        gera(" ", "DALLOC", to_string(memoryPosition), to_string(count));
        memoryPosition = memoryPosition -  count;
    }
    gera(" ", "RETURN", "", "");


}


void analysisProcedure() {
    getNextToken();
    if (token.getTypeString() == "sidentificador") {
        if(!symboltable->contains(token.getLexeme())){
            symboltable->push(token.getLexeme(), "L", "procedimento", to_string(label));
            gera(to_string(label), "NULL", "", "");
            label++;
            getNextToken();
            if(token.getTypeString() == "sponto_virgula"){
                blockAnalysis();
            } else {
                cout << 2 << endl;
                throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Procedimento ja declarado na linha: " + std::to_string(lexer.getCurrentLine()));
        }


    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
    int count = symboltable->cutStack();
    if(count > 0) {
        gera(" ", "DALLOC", to_string(memoryPosition), to_string(count));
        memoryPosition = memoryPosition -  count;
    }
    gera(" ", "RETURN", "", "");


}

void analysisSubroutine() {
    int flag, auxLabel;
    flag = 0;
    if(token.getTypeString() == "sprocedimento" || token.getTypeString() == "sfuncao"){
        auxLabel = label;
        gera(" ", "JMP", to_string(label), "");
        label++;
        flag = 1;
    }

    while (token.getTypeString() == "sprocedimento" || token.getTypeString() == "sfuncao") {
        if (token.getTypeString() == "sprocedimento") {
            analysisProcedure();
        } else {
            analysisFunction();
        }
        if(token.getTypeString() == "sponto_virgula") {
            getNextToken();
        } else {
            cout << 3 << endl;
            throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
        }
    }
    if(flag == 1){
        gera(to_string(auxLabel), "NULL", "", "");
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
    int count = 0;
    do {
        if (token.getTypeString() == "sidentificador") {
            if (!symboltable->containsVar(token.getLexeme())) {
                symboltable->push(token.getLexeme(), "", "var", to_string(memoryPosition + count));
                count++;
                getNextToken();
                if (token.getTypeString() == "svirgula" || token.getTypeString() == "sdoispontos") {
                    if (token.getTypeString() == "svirgula") {
                        getNextToken();
                        if (token.getTypeString() == "sdoispontos") {
                            throw std::runtime_error("Erro de Sintaxe! Padrao indevido na linha: " +
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
                throw std::runtime_error("Variavel ja declarada na linha: " +
                                         std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Esperava-se 'identificador' na linha: " +
                                     std::to_string(lexer.getCurrentLine()));
        }

    } while (token.getTypeString() != "sdoispontos");
    gera(" ", "ALLOC", to_string(memoryPosition), to_string(count));
    memoryPosition += count;
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
                    cout << 4 << endl;
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
    variablesDeclarationAnalysis();
    analysisSubroutine();
    commandsAnalysis();
}

int main() {
    try {
        if (token.getTypeString() == "sprograma"){
            getNextToken();
            if(token.getTypeString() == "sidentificador"){
                symboltable->push(token.getLexeme(), "", "programa", "");
                getNextToken();
                if(token.getTypeString() == "sponto_virgula"){
                    gera(" ", "START", "", "");
                    gera(" ", "ALLOC", "0", "1");
                    blockAnalysis();
                    if(token.getTypeString() == "sponto"){
                        int count = symboltable->cutStack();
                        cout << "Numero de variaveis Globais - " + to_string(count) << endl;
                        gera(" ", "DALLOC", to_string(memoryPosition), to_string(count));
                        getNextToken();
                        if(token.getTypeString() == "endfile"){
                            cout << "Compilado com sucesso!" << endl;
                            outputFile << endl << " ------ Compilado com sucesso! --------" << endl << endl;
                            symboltable->printStack();
                            gera(" ", "DALLOC", "0", "1");
                            gera(" ", "HLT", "", "");

                        } else
                            throw std::runtime_error("Simbolos invalidos apos o fim do programa na linha: " + std::to_string(lexer.getCurrentLine()));
                    } else {
                        throw std::runtime_error("Espera-se '.' na linha: " + std::to_string(lexer.getCurrentLine()));
                    }
                } else {
                    cout << 5 << endl;
                    throw std::runtime_error("Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            } else {
                throw std::runtime_error("Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'programa' na linha: " + std::to_string(lexer.getCurrentLine()));
        }


    } catch (const std::exception& e) {
        // Em caso de erro, grava a mensagem de erro no arquivo
        cout << "Erro: " <<   e.what() << endl;
        outputFile << lexer.getCurrentLine() << endl << "Erro: " <<   e.what() << endl;
        assemblyFile.close();
        assemblyFile.open("assembly.txt", ofstream::out | ofstream::trunc);
        assemblyFile.close();

    }
    outputFile.close();
    assemblyFile.close();
    return 0;
}
