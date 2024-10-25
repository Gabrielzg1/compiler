#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <algorithm>
#include "Lexical/Lexical.h"
#include "Token/Token.h"
using namespace std;

// Variáveis Globais
Lexical lexer("code.txt");
SymbolTable* symboltable = new SymbolTable();
Token token = lexer.getNextToken();
vector<string> auxiliar;

// Arquivo de saída para salvar os resultados
ofstream outputFile("output.txt");

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
void simpleExpressionAnalysis();

void getNextToken() {
    //cout << token.getTypeString() + " -> " + token.getLexeme() << endl;
    token = lexer.getNextToken();
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
                    } else {
                        throw runtime_error("Tipo invalido para o token '" + expr + "'.");
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
                    throw runtime_error("Operadores aritmeticos requerem inteiros.");
                }

                // Resultado também é inteiro
                typeStack.push("inteiro");

            } else if (expr == "+u" || expr == "-u") {
                // Operadores aritméticos unários: operando deve ser inteiro
                if (typeStack.empty()) throw runtime_error("Operandos insuficientes.");
                string operand = typeStack.top(); typeStack.pop();

                if (operand != "inteiro") {
                    throw runtime_error("Operadores unarios requerem inteiros.");
                }

                // Resultado também é inteiro
                typeStack.push("inteiro");

            } else if (expr == "=" || expr == "!=") {
                // Operadores de igualdade: ambos operandos devem ser do mesmo tipo
                if (typeStack.size() < 2) throw runtime_error("Operandos insuficientes.");
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != left) {
                    throw runtime_error("Operadores de igualdade requerem operandos do mesmo tipo.");
                }

                // Resultado é booleano
                typeStack.push("booleano");

            } else if (expr == "<" || expr == ">" || expr == "<=" || expr == ">=") {
                // Operadores relacionais: ambos operandos devem ser inteiros
                if (typeStack.size() < 2) throw runtime_error("Operandos insuficientes.");
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "inteiro" || left != "inteiro") {
                    throw runtime_error("Operadores relacionais requerem inteiros.");
                }

                // Resultado é booleano
                typeStack.push("booleano");

            } else if (expr == "e" || expr == "ou") {
                // Operadores lógicos binários: ambos operandos devem ser booleanos
                if (typeStack.size() < 2) throw runtime_error("Operandos insuficientes.");
                string right = typeStack.top(); typeStack.pop();
                string left = typeStack.top(); typeStack.pop();

                if (right != "booleano" || left != "booleano") {
                    throw runtime_error("Operadores logicos requerem booleanos.");
                }

                // Resultado também é booleano
                typeStack.push("booleano");

            } else if (expr == "nao") {
                // Operador unário lógico: operando deve ser booleano
                if (typeStack.empty()) throw runtime_error("Operandos insuficientes.");
                string operand = typeStack.top(); typeStack.pop();

                if (operand != "booleano") {
                    throw runtime_error("Operador 'nao' requer booleano.");
                }

                // Resultado também é booleano
                typeStack.push("booleano");
            }
        }
    }

    // Ao final, deve sobrar apenas um tipo na pilha
    if (typeStack.size() != 1) throw runtime_error("Erro: expressão malformada.");

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
    getNextToken();
}

void procedureCallAnalysis(){
    // Geracao de código
}

void atribAnalysis(const string& type) {
    getNextToken();

    // Criar e passar por referência o vetor de strings para a expressão infix
    std::vector<std::string> infixExpression;
    expressionAnalysis(infixExpression);


    vector<string> postfix = symboltable->toPostFix(infixExpression);

    cout << "Posfixa: ";
    for (const string& token : postfix) {
        cout << token << " ";
    }
    cout << endl;

    string expressionType = inferType(postfix);

    cout << endl << expressionType << endl;
    if(expressionType != type){
        throw std::runtime_error("Atribuicao de tipos diferentes na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}

void factorAnalysis(std::vector<std::string>& infixExpression) {
    if (token.getTypeString() == "sidentificador") {
        if (symboltable->containsProcFunc(token.getLexeme())) {
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
            if(symboltable->containsVar(token.getLexeme()) && symboltable->getType(token.getLexeme()) == "inteiro"){
                /* Inserir Lógica de busca geracao de código -> pega a primeira ocorrencia da variavel
                 *
                 * /////
                 *
                */
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
            if(symboltable->containsVar(token.getLexeme()) && symboltable->getType(token.getLexeme()) == "inteiro"){
                /* Inserir Lógica de busca geracao de código -> pega a primeira ocorrencia da variavel
                 *
                 * /////
                 *
                */
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
    if (symboltable->containsProcFunc(token.getLexeme())) {
        string type = symboltable->getType(token.getLexeme());
        getNextToken();

        // Verificar se é uma atribuição ou chamada de procedimento com base no tipo e token atual
        if ((token.getTypeString() == "satribuicao") && (type == "inteiro" || type == "booleano")) {
            atribAnalysis(type);
        } else if (type == "procedimento") {
            procedureCallAnalysis();
        } else {
            // Tipo inválido para atribuição ou chamada de procedimento
            throw std::runtime_error("Tipo inválido na atribuição/chamada de procedimento na linha " + std::to_string(lexer.getCurrentLine()));
        }
    } else {
        // Variável não declarada
        throw std::runtime_error("Variavel nao declarada na linha " + std::to_string(lexer.getCurrentLine()));
    }
}


void ifAnalysis() {
    getNextToken();

    // Criar e passar por referência o vetor de strings para a expressão infix
    std::vector<std::string> infixExpression;
    expressionAnalysis(infixExpression);
    vector<string> postfix = symboltable->toPostFix(infixExpression);

    string expressionType = inferType(postfix);
    if(expressionType != "booleano"){
        throw std::runtime_error("Atribuicao de tipos diferentes na linha: " + std::to_string(lexer.getCurrentLine()));
    }

    if (token.getTypeString() == "sentao") {
        getNextToken();
        simpleCommand();
        if (token.getTypeString() == "ssenao") {
            getNextToken();
            simpleCommand();
        }
    } else {
        throw std::runtime_error("Erro de Sintaxe! Espera-se 'entao' na linha: " + std::to_string(lexer.getCurrentLine()));
    }
}

void whileAnalysis() {
    getNextToken();

    // Criar e passar por referência o vetor de strings para a expressão infix
    std::vector<std::string> infixExpression;
    expressionAnalysis(infixExpression);
    vector<string> postfix = symboltable->toPostFix(infixExpression);

    string expressionType = inferType(postfix);
    if(expressionType != "booleano"){
        throw std::runtime_error("Atribuicao de tipos diferentes na linha: " + std::to_string(lexer.getCurrentLine()));
    }

    if (token.getTypeString() == "sfaca") {
        getNextToken();
        simpleCommand();
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
        if(!symboltable->containsProcFunc(token.getLexeme())){
            symboltable->push(token.getLexeme(), "L", "function", "");
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
    symboltable->cutStack();
}

void analysisProcedure() {
    getNextToken();
    if (token.getTypeString() == "sidentificador") {
        if(!symboltable->containsProcFunc(token.getLexeme())){
            symboltable->push(token.getLexeme(), "L", "procedimento", "");
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
    symboltable->cutStack();
}

void analysisSubroutine() {
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
            if (!symboltable->containsVar(token.getLexeme())) {
                symboltable->push(token.getLexeme(), "", "var", "");
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
                    blockAnalysis();
                    if(token.getTypeString() == "sponto"){
                        getNextToken();
                        if(token.getTypeString() == "endfile"){
                            cout << "Compilado com sucesso!" << endl;
                            outputFile << endl << " ------ Compilado com sucesso! --------" << endl << endl;
                            symboltable->printStack();
                        } else
                            throw std::runtime_error("ERRO sintatico: " + std::to_string(lexer.getCurrentLine()));
                    } else {
                        throw std::runtime_error("Erro de Sintaxe! Espera-se '.' na linha: " + std::to_string(lexer.getCurrentLine()));
                    }
                } else {
                    cout << 5 << endl;
                    throw std::runtime_error("Erro de Sintaxe! Espera-se ';' na linha: " + std::to_string(lexer.getCurrentLine()));
                }
            } else {
                throw std::runtime_error("Erro de Sintaxe! Espera-se 'identificador' na linha: " + std::to_string(lexer.getCurrentLine()));
            }
        } else {
            throw std::runtime_error("Erro de Sintaxe! Espera-se 'programa' na linha: " + std::to_string(lexer.getCurrentLine()));
        }


    } catch (const std::exception& e) {
        // Em caso de erro, grava a mensagem de erro no arquivo
        cout << "Erro: " <<   e.what() << endl;
        outputFile << lexer.getCurrentLine() << endl << "Erro: " <<   e.what() << endl;
    }
    outputFile.close();
    return 0;
}
