#include <iostream>
#include "Lexical/Lexical.h"
#include "Token/Token.h"
using namespace std;
int main() {
    try {
        Lexical lexer("code.txt");
        SymbolTable* symboltable = new SymbolTable();
        while(true){
            Token token = lexer.getNextToken();
            if(token.getTypeString(token) == "sidentificador"){
                symboltable->push(token.getLexeme(), 0, "var", 0);

            }
            if(token.getLexeme() == "endfile"){
                break;
            }
            cout << token.toString() << endl;
        }
        cout << symboltable->peek()->name << endl;



    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}