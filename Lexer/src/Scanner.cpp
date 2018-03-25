//
// Created by spencer on 2/23/18.
//

#include "../include/Scanner.h"

bool Scanner::checkSetReservedWord(Token& token)
{
    auto type = m_reservedWords.find(token.value);
    if (type != m_reservedWords.end()) {
        token.type = type->second;
        return true;
    }
    return false;
}

void Scanner::loadSourceFile(const char* filepath){
    depth++;
    m_source_files.push(std::make_shared<std::ifstream>(filepath));
    m_current_file = m_source_files.top();
}

void Scanner::popSourceFile()
{
    m_source_files.pop();
    depth--;
    if (!m_source_files.empty()) {
        m_current_file = m_source_files.top();
    } else {
        m_current_file = nullptr;
    }
}

char Scanner::getNextChar()
{
    return m_current_file->get();
}

void Scanner::unGetChar(char character)
{
    m_current_file->putback(character);
}

Token Scanner::getNextToken()
{
    Token token;
    token.value += getNextChar();
    char c;
    bool extraCharNeedsPutback = true;

    // handle end-of-file
    if(token.value.at(0) == EOF){
        extraCharNeedsPutback = false;
        popSourceFile();
        if(m_source_files.empty()) {
            token.value = "end";
            end = true;
        }
        else {
            token.value = "eof";
        }
    }

    // WHITESPACE TOKEN TYPE
    else if (isWhiteSpace(token.value[0])) {
        token.type = TokenType::WS;
        while (isWhiteSpace(c = getNextChar())) {
            token.value += c;
        }
    }

    // newline char
    else if (token.value[0] == '\n') {
        token.type = TokenType::LF;
        extraCharNeedsPutback = false;
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
        token.value.pop_back();
        while ((c = getNextChar()) != '"' && c != '\n') {
            token.value += c;
        }
        if (c == '"') {
            extraCharNeedsPutback = false;
        }
        else if (c == '\n')
            std::cerr << "Fatal Lexical Error: String literals cannot contain '\\n'" << std::endl;
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
                // does ternary belong here?
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

        // set TokenType based on value
        token.type = tokenSymbolFromValue(token.value);
    }

    // preprocessor symbol
    else if (token.value[0] == '#') {
        token.type = TokenType::PREPROCESSOR_SYMBOL;
        extraCharNeedsPutback = false;
    }


    if (logRawTokens) {
        logTokenCreation(token,depth);
    }
    if (extraCharNeedsPutback) {
        unGetChar(c);
    }
    if (token.type == TokenType::WS) {
        return getNextToken();
    }
    return token;
}

Scanner::Scanner() {
}

//TODO delete allocated ifstream when source_files are removed from stack
Scanner::Scanner(char* filepath)
{
    loadSourceFile(filepath);
}

Scanner::Scanner(char *filename, bool doRawTokenLogging)
{
    loadSourceFile(filename);
    logRawTokens = doRawTokenLogging;
}
