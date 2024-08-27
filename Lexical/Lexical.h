#ifndef LEXICAL_H
#define LEXICAL_H

#include <vector>
#include <string>
#include <fstream>
#include "../Token/Token.h"
#include "../SymbolTable/SymbolTable.h"

class Lexical {
public:
    Lexical(const std::string& filename);
    ~Lexical();

    void analyze();
    const std::vector<Token>& getTokens() const;
    Token getNextToken();

private:
    std::ifstream sourceFile;
    std::vector<Token> tokens;

    void consumeWhitespaceAndComments();
    bool isLetter(char ch) const;
    bool isDigit(char ch) const;


    SymbolTable *symbolTable;
};

#endif // LEXICAL_H
