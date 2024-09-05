#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>
#include <system_error>

enum TokenType {
    sprograma,
    sinicio,
    sfim,
    sprocedimento,
    sfuncao,
    sse,
    sentao,
    ssenao,
    senquanto,
    sfaca,
    satribuicao,
    sescreva,
    sleia,
    svar,
    sinteiro,
    sbooleano,
    sidentificador,
    snumero,
    sponto,
    sponto_virgula,
    svirgula,
    sabre_parenteses,
    sfecha_parenteses,
    smaior,
    smaiorig,
    sig,
    smenor,
    smenorig,
    sdif,
    smais,
    smenos,
    smult,
    sdiv,
    se,
    sou,
    snao,
    sdoispontos,
    sverdadeiro,
    sfalso,
    endfile,
    TOKEN_UNKNOWN
};

class Token : public std::error_code {
public:
    Token(TokenType type, const std::string& lexeme);

    TokenType getType() const;
    std::string getLexeme() const;
    std::string toString() const;
    std::string getTypeString(Token token) const;


private:
    TokenType type;
    std::string lexeme;

    std::string tokenTypeToString(TokenType type) const;

};

// Função para criar um Token com base em um lexema
Token getToken(const std::string& lexema);

// Mapa global que associa lexemas a tokens
extern std::unordered_map<std::string, TokenType> lexemaToToken;

#endif // TOKEN_H
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>

enum TokenType {
    sprograma,
    sinicio,
    sfim,
    sprocedimento,
    sfuncao,
    sse,
    sentao,
    ssenao,
    senquanto,
    sfaca,
    satribuicao,
    sescreva,
    sleia,
    svar,
    sinteiro,
    sbooleano,
    sidentificador,
    snumero,
    sponto,
    sponto_virgula,
    svirgula,
    sabre_parenteses,
    sfecha_parenteses,
    smaior,
    smaiorig,
    sig,
    smenor,
    smenorig,
    sdif,
    smais,
    smenos,
    smult,
    sdiv,
    se,
    sou,
    snao,
    sdoispontos,
    sverdadeiro,
    sfalso,
    TOKEN_UNKNOWN
};

class Token {
public:
    Token(TokenType type, const std::string& lexeme);

    TokenType getType() const;
    std::string getLexeme() const;
    std::string toString() const;

private:
    TokenType type;
    std::string lexeme;

    std::string tokenTypeToString(TokenType type) const;
};

// Função para criar um Token com base em um lexema
Token getToken(const std::string& lexema);

// Mapa global que associa lexemas a tokens
extern std::unordered_map<std::string, TokenType> lexemaToToken;

#endif // TOKEN_H
