#ifndef LEXICAL_H
#define LEXICAL_H

#include <vector>
#include <string>
#include <fstream>
#include "../Token/Token.h"

class Lexical {
public:
    Lexical(const std::string& filename);
    ~Lexical();

    void analyze();
    const std::vector<Token>& getTokens() const;

private:
    std::ifstream sourceFile;
    std::vector<Token> tokens;

    void consumeWhitespaceAndComments();
    Token getNextToken();
    bool isLetter(char ch) const;
    bool isDigit(char ch) const;
};

#endif // LEXICAL_H
