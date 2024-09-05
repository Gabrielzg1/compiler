// Lexical.cpp
#include "Lexical.h"
#include "../SymbolTable/SymbolTable.h"
#include <cctype>
#include <iostream>
// Construtor que abre o arquivo fonte
Lexical::Lexical(const std::string& filename) { // Inicializa a linha como 1
    sourceFile.open(filename);
    if (!sourceFile.is_open()) {
        throw std::runtime_error("Failed to open source file");
    }
}
// Destrutor que fecha o arquivo fonte
Lexical::~Lexical() {
    if (sourceFile.is_open()) {
        sourceFile.close();
    }
}
// Função principal de análise
void Lexical::analyze() {
    while (sourceFile) {
        consumeWhitespaceAndComments();
        if (sourceFile.peek() != EOF) {
            Token token = getNextToken();
            tokens.push_back(token);
        }
    }
}
// Função para consumir espaços e comentários
void Lexical::consumeWhitespaceAndComments() {
    char ch;
    while (sourceFile.get(ch)) {
        if (ch == '{') {
            // Ignorar comentários entre '{' e '}'
            while (sourceFile.get(ch) && ch != '}') {
                if(ch == '\n') {
                    this->line++;
                }
            }
        } else if (isspace(ch)) {
            if(ch == '\n') {
                this->line++;
            }
            continue;
        } else {
            sourceFile.putback(ch);
            break;
        }
    }
}

// Função que identifica e retorna o próximo token
Token Lexical::getNextToken() {
    //consumeWhitespaceAndComments();
    std::string lexeme;
    char ch;

    sourceFile.get(ch);
    // Identificadores e palavras-chave
    if (isLetter(ch)) {
        // Verifica se o identificador começa com '_'
        if (ch == '_') {
            throw std::runtime_error("Identificadores nao podem começar com '_'. Identificador invalido. Linha: " + std::to_string(line));
        }
        lexeme += ch;
        while (sourceFile.get(ch) && (isLetter(ch) || isDigit(ch) || ch == '_')) {
            lexeme += ch;
        }
        sourceFile.putback(ch);
        // Verifica se o lexema é uma palavra reservada
        if (lexeme == "programa") return Token(sprograma, lexeme);
        else if (lexeme == "se") return Token(sse, lexeme);
        else if (lexeme == "entao") return Token(sentao, lexeme);
        else if (lexeme == "senao") return Token(ssenao, lexeme);
        @@ -175,4 +180,8 @@
// Retorna a lista de tokens
        const std::vector<Token>& Lexical::getTokens() const {
            return tokens;
        }
        int Lexical::getCurrentLine() const {
            return line;
        }