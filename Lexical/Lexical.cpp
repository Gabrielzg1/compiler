#include "Lexical.h"
#include <cctype>

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
            while (ch != '}' && sourceFile.get(ch)) {}
        } else if (!isspace(ch)) {
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
        auto token = getToken(lexeme);
        if (token.getType() == TOKEN_UNKNOWN) {
            // Se não for uma palavra reservada, trata como identificador
            return Token(sidentificador, lexeme);
        }
        return token;
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
            // Qualquer outro símbolo desconhecido é tratado como um identificador
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
