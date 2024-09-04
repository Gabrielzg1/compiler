#include "Token.h"
#include <iostream>

// Construtor da classe Token
Token::Token(TokenType type, const std::string& lexeme)
        : type(type), lexeme(lexeme) {}

// Retorna o tipo do token
TokenType Token::getType() const {
    return type;
}
std::string Token::getTypeString() const {
    return tokenTypeToString(type);
}

// Retorna o lexema do token
std::string Token::getLexeme() const {
    return lexeme;
}

// Converte o token em string para facilitar a depuração
std::string Token::toString() const {
    return "Token(" + lexeme + ", \"" + tokenTypeToString(type) + "\")";
}

// Converte o tipo do token em string
std::string Token::tokenTypeToString(TokenType type) const {
    switch(type) {
        case sprograma: return "sprograma";
        case sinicio: return "sinicio";
        case sfim: return "sfim";
        case sprocedimento: return "sprocedimento";
        case sfuncao: return "sfuncao";
        case sse: return "sse";
        case sentao: return "sentao";
        case ssenao: return "ssenao";
        case senquanto: return "senquanto";
        case sfaca: return "sfaca";
        case satribuicao: return "satribuicao";
        case sescreva: return "sescreva";
        case sleia: return "sleia";
        case svar: return "svar";
        case sinteiro: return "sinteiro";
        case sbooleano: return "sbooleano";
        case sidentificador: return "sidentificador";
        case snumero: return "snumero";
        case sponto: return "sponto";
        case sponto_virgula: return "sponto_virgula";
        case svirgula: return "svirgula";
        case sabre_parenteses: return "sabre_parenteses";
        case sfecha_parenteses: return "sfecha_parenteses";
        case smaior: return "smaior";
        case smaiorig: return "smaiorig";
        case sig: return "sig";
        case smenor: return "smenor";
        case smenorig: return "smenorig";
        case sdif: return "sdif";
        case smais: return "smais";
        case smenos: return "smenos";
        case smult: return "smult";
        case sdiv: return "sdiv";
        case se: return "se";
        case sou: return "sou";
        case snao: return "snao";
        case sdoispontos: return "sdoispontos";
        case sverdadeiro: return "sverdadeiro";
        case sfalso: return "sfalso";
        default: return "TOKEN_UNKNOWN";
    }
}

// Mapa global que associa lexemas a tokens
std::unordered_map<std::string, TokenType> lexemaToToken = {
        {"programa", sprograma},
        {"início", sinicio},
        {"fim", sfim},
        {"procedimento", sprocedimento},
        {"funcao", sfuncao},
        {"se", sse},
        {"entao", sentao},
        {"senao", ssenao},
        {"enquanto", senquanto},
        {"faca", sfaca},
        {":=", satribuicao},
        {"escreva", sescreva},
        {"leia", sleia},
        {"var", svar},
        {"inteiro", sinteiro},
        {"booleano", sbooleano},
        {"identificador", sidentificador},
        {"número", snumero},
        {".", sponto},
        {";", sponto_virgula},
        {",", svirgula},
        {"(", sabre_parenteses},
        {")", sfecha_parenteses},
        {">", smaior},
        {">=", smaiorig},
        {"=", sig},
        {"<", smenor},
        {"<=", smenorig},
        {"!=", sdif},
        {"+", smais},
        {"-", smenos},
        {"*", smult},
        {"div", sdiv},
        {"e", se},
        {"ou", sou},
        {"nao", snao},
        {":", sdoispontos},
        {"verdadeiro", sverdadeiro},
        {"falso", sfalso}
};

// Função para criar um Token com base em um lexema
Token getToken(const std::string& lexema) {
    auto it = lexemaToToken.find(lexema);
    if (it != lexemaToToken.end()) {
        return Token(it->second, lexema);
    } else {
        return Token(TOKEN_UNKNOWN, lexema);
    }
}
