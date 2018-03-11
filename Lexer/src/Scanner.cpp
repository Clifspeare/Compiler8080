//
// Created by spencer on 2/23/18.
//

#include "../include/Scanner.h"

void populateReservedWordMap(std::map<std::string, TokenType> &reservedWordMap)
{
        reservedWordMap = {
                std::make_pair("int", TokenType::INT),
                std::make_pair("double", TokenType::DOUBLE),
                std::make_pair("float", TokenType::FLOAT),
                std::make_pair("short", TokenType::SHORT),
                std::make_pair("long", TokenType::LONG),
                std::make_pair("char", TokenType::CHAR),
                std::make_pair("bool", TokenType::BOOL),
                std::make_pair("void", TokenType::VOID),
                std::make_pair("if", TokenType::IF),
                std::make_pair("else", TokenType::ELSE),
                std::make_pair("return", TokenType::RETURN),
                std::make_pair("struct", TokenType::STRUCT),
                std::make_pair("while", TokenType::WHILE),
                std::make_pair("for", TokenType::FOR),
                std::make_pair("volatile", TokenType::VOLATILE),
                std::make_pair("enum", TokenType::ENUM),
                std::make_pair("extern", TokenType::EXTERN),
                std::make_pair("static", TokenType::STATIC),
                std::make_pair("continue", TokenType::CONTINUE),
                std::make_pair("const", TokenType::CONST),
                std::make_pair("auto", TokenType::AUTO),
                std::make_pair("typedef", TokenType::TYPEDEF),
                std::make_pair("default", TokenType::DEFAULT),
                std::make_pair("register", TokenType::REGISTER),
                std::make_pair("switch", TokenType::SWITCH),
                std::make_pair("case", TokenType::CASE),
                std::make_pair("union", TokenType::UNION),
                std::make_pair("signed", TokenType::SIGNED),
                std::make_pair("unsigned", TokenType::UNSIGNED),
                std::make_pair("do", TokenType::DO),
                std::make_pair("break", TokenType::BREAK),
                std::make_pair("goto", TokenType::GOTO)
        };
}

bool Scanner::checkSetReservedWord(Token& token)
{
    auto type = m_reservedWords.find(token.value);
    if (type != m_reservedWords.end()) {
        token.type = type->second;
        return true;
    }
    return false;
}

bool Scanner::checkSetSymbol(const Token& token)
{
    // TODO: Make this work.
//    if (token.value == '!')
//        token.type = TokenType::
//            c == '{' ||
//            c == '}' ||
//            c == '(' ||
//            c == ')' ||
//            c == '[' ||
//            c == ']' ||
//            c == '+' ||
//            c == '-' ||
//            c == '%' ||
//            c == '/' ||
//            c == '*' ||
//            c == '&' ||
//            c == '<' ||
//            c == '>' ||
//            c == '=' ||
//            c == '^' ||
//            c == '|' ||
//            c == '~' ||
//            c == ';' ||
//            c == ',' ||
//            c == '.' ||
//            c == ':' ||
//            c == '?'
//            )
//    {
//        return true;
//    }
    return false;
};

char Scanner::getNextChar()
{
    m_current_file->index++;
    return m_current_file->buffer.get();
}

void Scanner::unGetChar(char character)
{
    m_current_file->index--;
    m_current_file->buffer.putback(character);
}

Token Scanner::getNextToken()
{

    Token token;
    token.value = getNextChar();
    if (token.value[0] == -1)
    token.type = TokenType::UNDEFINED;
    char c;

    // WHITESPACE TOKEN TYPE
    if (isWhiteSpace(token.value[0])) {
        token.type = TokenType::WS;
        while (isWhiteSpace(c = getNextChar())) {
            token.value += c;
        }
    }
    // RESERVED WORD / IDENTIFIER TOKEN TYPE
    else if (token.value[0] == '_' || isalpha(token.value[0])) {
        token.type = TokenType::ID;
        while ((c = getNextChar()) == '_' || isalnum(c)) {
            token.value += c;
        }
        checkSetReservedWord(token);
    }
    // NUMBER/CONSTANT TOKEN TYPE
    else if (isdigit(token.value[0]) || c == '.') {
        bool decimalSeen = false;
        token.type = TokenType::CONSTANT;
        while ( isdigit((c = getNextChar())) || (c == '.' && !decimalSeen) ) {
            //TODO implement numeric constant logic
            if(c == '.'){
                decimalSeen==true;
            }
            token.value += c;
        }
    }
    // STRING TOKEN TYPE
    else if (token.value[0] == '"') {
        token.type = TokenType::LITERAL;
        while ((c = getNextChar()) != '"' && c != '\n') {
            token.value += c;
        }
    }
    // SYMBOLS / SPECIAL TOKEN TYPE
    else if(isSymbol(token.value[0])) {
        if (isSymbol(c = getNextChar())) {
            token.value += c;
            getNextChar();
        }
        checkSetSymbol(token);
    }

    else if (token.value[0] == '#') {
        token.type = TokenType::PREPROCESSOR_SYMBOL;
        getNextChar();
    }

    logTokenCreation(token);
    unGetChar(c);

    return token;
}

Scanner::Scanner() : m_current_file() {
    populateReservedWordMap(m_reservedWords);
}

Scanner::Scanner(char* filepath) : m_current_file() {
    source_file currentFile;
    currentFile.buffer = std::ifstream(filepath);
    m_source_files.push(currentFile);
    m_current_file = &m_source_files.top();
    populateReservedWordMap(m_reservedWords);
}
