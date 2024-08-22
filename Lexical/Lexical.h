#ifndef COMPILER_LEXICAL_H
#define COMPILER_LEXICAL_H
#include <cstdio>    // Para FILE*, fopen, fgetc, fclose


namespace Lexical {
    class Lexical {
    public:
        Lexical(); // Construtor
        char getChar(); // Método que retorna um inteiro
        int printFile();
        FILE* file;
    };

} // namespace Lexical

#endif //COMPILER_LEXICAL_H
