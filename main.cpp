#include <iostream>
#include "Lexical/Lexical.h"
#include "Token/Token.h"
using namespace std;
int main() {
    try {
        Lexical lexer("code.txt");
        while(true){
            Token token = lexer.getNextToken();
            if(token.getLexeme() == "endfile"){
                break;
            }
            cout << token.toString() << endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}