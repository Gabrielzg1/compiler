#ifndef LEXICAL_H
#define LEXICAL_H
#include <string>
#include <fstream>
#include <vector>
#include "../Token/Token.h"
#include "../SymbolTable/SymbolTable.h"
class Lexical {
public:
    Lexical(const std::string& filename);
    ~Lexical();
    void analyze();
    const std::vector<Token>& getTokens() const;
    int getCurrentLine() const;
    Token getNextToken();

private:
    void consumeWhitespaceAndComments();
    bool isLetter(char ch) const;
    bool isDigit(char ch) const;
    std::ifstream sourceFile;
    std::vector<Token> tokens;
    int line = 1;

};

#endif // LEXICAL_H