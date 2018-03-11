#ifndef COMPILER8080_SCANNER_H
#define COMPILER8080_SCANNER_H

#include "token_types.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <stack>
#include <array>
#include <iostream>
#include "utility_functions.h"

// Need to write fstream that reads in spaces and new lines in getNextToken.

struct source_file {
    std::ifstream buffer;
    int index = 0;
};

class Scanner {
public:
    Scanner();

    Scanner(char* filename);

    // Pushes new source file onto the stack.
    void loadSourceFile(char* filepath);

    Token getNextToken();
    Token buildToken();
private:
    // implementation of token creation (from file) with state transitions, etc.
    bool checkSetReservedWord(Token& token);
    bool checkSetSymbol(Token& token);

    char getNextChar();
    void unGetChar(char Character);

    std::vector<Token> m_tokens;
    std::vector<std::string> m_literals;
    std::vector<std::string> m_constants;
    std::vector<std::string> m_identifiers;
    std::map<std::string, TokenType> m_reservedWords;

    source_file *m_current_file;
    std::stack<source_file> m_source_files;
};


#endif //COMPILER8080_SCANNER_H
