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
        Token token = getNextToken();
        tokens.push_back(token);
    }
}
// Função para consumir espaços e comentários
void Lexical::consumeWhitespaceAndComments() {
    char ch;
    while (sourceFile.get(ch)) {
        if (ch == '{') {
            int openLine = this->line; // Armazena a linha onde o comentário foi aberto
            bool closed = false; // Flag para verificar se o comentário foi fechado
            // Ignorar comentários entre '{' e '}'
            while (sourceFile.get(ch)) {
                if (ch == '{') {
                    std::cerr << "Erro: Comentario aberto na linha " << openLine << " nao foi fechado." << std::endl;
                }
                if (ch == '}') {
                    closed = true;
                    break;
                }
                if (ch == '\n') {
                    this->line++;
                }
            }
            if (!closed) {
                std::cerr << "Erro: Comentário aberto na linha " << openLine << " não foi fechado." << std::endl;
            }
        } else if (isspace(ch)) {
            if (ch == '\n') {
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
    consumeWhitespaceAndComments();
    if (sourceFile.eof()) {
        return Token(endfile, "endfile");
    }

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
            // Adicionar o identificador à tabela de símbolos
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
        throw std::runtime_error("Erro: Operador invalido '!'. Linha: " + std::to_string(line));
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
        case '=': return Token(sig, lexeme);
        case '_': throw std::runtime_error("'_' utilizado de maneira incorreta. Linha: " + std::to_string(line));
        default:
            throw std::runtime_error("Simbolo desconhecido '" + lexeme + "'. Linha: " + std::to_string(line));
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
int Lexical::getCurrentLine() const {
    return line;
}