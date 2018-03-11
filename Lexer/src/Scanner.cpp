//
// Created by spencer on 2/23/18.
//

#include "../include/Scanner.h"

bool isSymbol(const char c)
{

};

bool isWhiteSpace(const char c)
{
    if (c == '\t' || c == ' ') {
        return true;
    }
}

bool Scanner::checkReservedWord(Token& token)
{
    //TODO: Check reserved words for matching token.value and set token.type.
    auto type = m_reserved.find(token.value);
    if (type != m_reserved.end()) {
        token.type = type->second;
        return true;
    }
    return false;
}

Token Scanner::getNextToken()
{

    Token token;
    char t_char = m_current_file->get();

    // WHITESPACE TOKEN TYPE
    if (isWhiteSpace(t_char)) {
        while (isWhiteSpace(t_char = m_current_file->get())) ; // no body
        m_current_file->putback(t_char);
    }
    // RESERVED WORD / IDENTIFIER TOKEN TYPE
    else if (isalpha(t_char)) {
        while ((t_char = m_current_file->get()) == '_' || isalnum(t_char)) {
            token.value += t_char;
        }
        if (!checkReservedWord(token)) {
            token.type = TokenType::ID;
        }
    }
    // NUMBER TOKEN TYPE
    else if (isdigit(t_char)) {
        while (isdigit((t_char = m_current_file->get())) || t_char == '.') {
            //TODO implement numeric constant logic
        }
    }
    // STRING TOKEN TYPE
    else if (t_char == '"') {

    }
    // SYMBOLS / SPECIAL TOKEN TYPE
    else if(isSymbol(t_char)) {

    }

    m_current_file->putback(t_char);
}

void Scanner::populateReservedWordMap()
{
    m_reserved = std::map<std::string, TokenType>({
    {TokenType::INT, "int"},
    {"double", TokenType::DOUBLE},
    {"float", TokenType::FLOAT},
    {"char", TokenType::CHAR},
    {"bool", TokenType::BOOL},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"return", TokenType::RETURN},
    {"struct", TokenType::STRUCT}
    });
}

Scanner::Scanner() {
    populateReservedWordMap();
}
