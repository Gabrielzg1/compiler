#include <iostream>
#include <cstdio>
#include "Lexical/Lexical.h"

using namespace std;

int main() {
    Lexical::Lexical lexical;
    char character;

    // Loop para ler caracteres até o fim do arquivo
    while ((character = lexical.getChar()) != EOF) {  // Correção: Parênteses posicionados corretamente
        if(character == '{') {  // Correção: Comparação de caractere
            cout << "inicio comentario";
        }
    }

    return 0;
}
