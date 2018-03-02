//
// Created by spencer on 2/23/18.
//

#ifndef COMPILER8080_SCANNER_H
#define COMPILER8080_SCANNER_H

#include "token_types.h"
#include <string>
#include <vector>
#include <fstream>

struct token {
    TokenType type;
    std::string* value;
};

class Scanner {
public:
    Scanner();
    Scanner(char* filename);

    token getNextToken();
private:
    // implementation of token creation (from file) with state transitions, etc.

    std::vector<token> m_tokens;
    std::vector<std::string> m_literals;
    std::vector<std::string> m_constants;
    std::vector<std::string> m_identifiers;
    std::ifstream m_input;
};


#endif //COMPILER8080_SCANNER_H
