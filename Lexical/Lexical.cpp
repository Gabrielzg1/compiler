#include "Lexical.h"
#include "../SymbolTable/SymbolTable.h"
#include <cctype>
#include <iostream>

// Construtor que abre o arquivo fonte
Lexical::Lexical(const std::string& filename) {
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
            while (sourceFile.get(ch) && ch != '}') {}
        } else if (isspace(ch)) {
            // Ignorar espaços, tabs e novas linhas
            continue;
        } else {
            sourceFile.putback(ch);
            break;
        }
    }
}

// Função que identifica e retorna o próximo token
Token Lexical::getNextToken() {
    std::string lexeme;
    char ch;

    sourceFile.get(ch);

    // Identificadores e palavras-chave
    if (isLetter(ch)) {
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
        else if (lexeme == "enquanto") return Token(senquanto, lexeme);
        else if (lexeme == "faca") return Token(sfaca, lexeme);
        else if (lexeme == "inicio") return Token(sinicio, lexeme);
        else if (lexeme == "fim") return Token(sfim, lexeme);
        else if (lexeme == "escreva") return Token(sescreva, lexeme);
        else if (lexeme == "leia") return Token(sleia, lexeme);
        else if (lexeme == "var") return Token(svar, lexeme);
        else if (lexeme == "inteiro") return Token(sinteiro, lexeme);
        else if (lexeme == "booleano") return Token(sbooleano, lexeme);
        else if (lexeme == "verdadeiro") return Token(sverdadeiro, lexeme);
        else if (lexeme == "falso") return Token(sfalso, lexeme);
        else if (lexeme == "procedimento") return Token(sprocedimento, lexeme);
        else if (lexeme == "funcao") return Token(sfuncao, lexeme);
        else if (lexeme == "div") return Token(sdiv, lexeme);
        else if (lexeme == "e") return Token(se, lexeme);
        else if (lexeme == "ou") return Token(sou, lexeme);
        else if (lexeme == "nao") return Token(snao, lexeme);
        else {
            // Adicionar o identificador à tabela de símbolo
           // symbolTable.push(new SymbolInfo{lexeme, 0, "", 0});
            return Token(sidentificador, lexeme); // Se não for palavra reservada, é identificador
        }
    }

    // Números
    if (isDigit(ch)) {
        lexeme += ch;
        while (sourceFile.get(ch) && isDigit(ch)) {
            lexeme += ch;
        }
        sourceFile.putback(ch);
        return Token(snumero, lexeme);
    }

    // Operadores compostos
    if (ch == ':') {
        lexeme += ch;
        if (sourceFile.peek() == '=') {
            sourceFile.get(ch);
            lexeme += ch;
            return Token(satribuicao, lexeme);
        }
        return Token(sdoispontos, lexeme);
    }
    if (ch == '<') {
        lexeme += ch;
        if (sourceFile.peek() == '=') {
            sourceFile.get(ch);
            lexeme += ch;
            return Token(smenorig, lexeme);
        }
        return Token(smenor, lexeme);
    }
    if (ch == '>') {
        lexeme += ch;
        if (sourceFile.peek() == '=') {
            sourceFile.get(ch);
            lexeme += ch;
            return Token(smaiorig, lexeme);
        }
        return Token(smaior, lexeme);
    }
    if (ch == '!') {
        lexeme += ch;
        if (sourceFile.peek() == '=') {
            sourceFile.get(ch);
            lexeme += ch;
            return Token(sdif, lexeme);
        }
        return Token(TOKEN_UNKNOWN, lexeme);  // Possivelmente erro de sintaxe
    }

    // Operadores simples e pontuação
    lexeme += ch;
    switch (ch) {
        case ';': return Token(sponto_virgula, lexeme);
        case '.': return Token(sponto, lexeme);
        case ',': return Token(svirgula, lexeme);
        case '(': return Token(sabre_parenteses, lexeme);
        case ')': return Token(sfecha_parenteses, lexeme);
        case '+': return Token(smais, lexeme);
        case '-': return Token(smenos, lexeme);
        case '*': return Token(smult, lexeme);
        default:
            if (isLetter(ch) || isDigit(ch) || ch == '_') {
                return Token(sidentificador, lexeme);
            }
            return Token(TOKEN_UNKNOWN, lexeme);
    }
}

// Função que verifica se um caractere é uma letra
bool Lexical::isLetter(char ch) const {
    return std::isalpha(static_cast<unsigned char>(ch));
}

// Função que verifica se um caractere é um dígito
bool Lexical::isDigit(char ch) const {
    return std::isdigit(static_cast<unsigned char>(ch));
}

// Retorna a lista de tokens
const std::vector<Token>& Lexical::getTokens() const {
    return tokens;
}
