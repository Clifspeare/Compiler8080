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
    return m_current_file->buffer->get();
}

void Scanner::unGetChar(char character)
{
    m_current_file->index--;
    m_current_file->buffer->putback(character);
}

Token Scanner::getNextToken()
{

    Token token;
    token.value += getNextChar();
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
                decimalSeen = true;
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
        c = getNextChar(); // get next char before handling, can push back later
        switch (token.value[0]) { //handle cases where symbol may consist of > 1 character
            case '-':
                if (c == '>') // pointer member referencing operator
                    token.value += c;
                if (c == '-') // decrement op
                    token.value += c;
                if (c == '=') // subtract assign
                    token.value += c;
            case '+':
                if (c == '+') // increment op
                    token.value += c;
                if (c == '=') // add assign
                    token.value += c;
            case '!':
                if (c == '=') // inequality
                    token.value += c;
            case '/':
                if (c == '=') // divide assign
                    token.value += c;
            case '*':
                if (c == '=') // multiply assign
                    token.value += c;
            case '%':
                if (c == '=') // mod assign
                    token.value += c;
            case '<':
                if (c == '<') { // this is one a few special characters that may be in symbols of 3 characters in size
                    token.value += c;
                    if ((c = getNextChar()) == '=') // bit shift left assign
                        token.value += c;
                    else // just bit shift left
                        unGetChar(c); // final check for ungetting c won't work for this 3-character symbol case
                }
                if (c == '=') // less-than bool assign
                    token.value += c;

            case '>':
                if (c == '>') { // bit shift right
                    token.value += c;
                    if ((c = getNextChar()) == '=') // as above
                        token.value += c;
                    else
                        unGetChar(c);
                }
                if (c == '=') // greater-than bool assign
                    token.value += c;
            case '=':
                if (c == '=') //equality
                    token.value += c;
            case '&':
                if (c == '&') // logic AND
                    token.value += c;
                if (c == '=') // bit AND assign
                    token.value += c;
            case '|':
                if (c == '|') // logic OR
                    token.value += c;
                if (c == '=') // bit NOT assign
                    token.value += c;
            // apparently ternary (? :) counts as a single operator in c.  how to handle THAT, i have NO IDEA.
            // leaving for now (later thoughts... maybe just walk ahead at instance of ? until we find ':')?
            // still not sure how we should represent.
            case '^':
                if (c == '=') // XOR assign
                    token.value += c;
            case '#':
                if (c == '#') // some preprocessor mumbo-jumbo
                    token.value += c;
            case '.':
                char third_char = getNextChar();
                if (c == '.' && third_char == '.') { // elipsis
                    token.value += c + third_char;
                } else {
                    unGetChar(third_char); // unget extra/unhandled 3rd char
                }
        }
        if (token.value.length() == 1) // only unget c if it was part of the symbol ( so part of next token)
            unGetChar(c);
//        if (isSymbol(c = getNextChar())) {
//            token.value += c;
//            getNextChar();
//        }
        // set TokenType based on value
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

Scanner::Scanner() {
    populateReservedWordMap(m_reservedWords);
}

//TODO delete allocated ifstream when source_files are removed from stack
Scanner::Scanner(char* filepath) {
    source_file currentFile;
    currentFile.buffer = new std::ifstream(filepath);
    m_source_files.push(currentFile);
    m_current_file = &m_source_files.top();
    populateReservedWordMap(m_reservedWords);
}
