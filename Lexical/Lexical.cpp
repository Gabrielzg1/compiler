#include "Lexical.h"
#include <iostream>
#include <cstdio>

using namespace std;
namespace Lexical {
    Lexical::Lexical() {
        this->file = fopen("code.txt", "r");

        if (this->file == nullptr) {
            std::cerr << "Não foi possível abrir o arquivo." << std::endl;
            return;
        }
        //fclose(arquivo);
    }

    int Lexical::printFile() {
        char character;
        while ((character = fgetc(this->file)) != EOF) {
            std::cout << character;  // Imprime o character lido na tela
        }
        return 0;
    }
    char Lexical::getChar() {
        return char(fgetc(this->file));
    }

} // namespace Lexical
