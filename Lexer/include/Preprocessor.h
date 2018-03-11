//
// Created by spencer on 3/4/18.
//

#ifndef COMPILER8080_PREPROCESSOR_H
#define COMPILER8080_PREPROCESSOR_H
#include "Scanner.h"
#include <algorithm>

struct Definition {
    std::string definition;
    Token token;
};

class Preprocessor {
public:
    Preprocessor();

    Token getNextToken();
private:
    void addDefinition(Token definition, Token value);

    Scanner m_scanner;

    std::vector<Definition> m_defines;
};


#endif //COMPILER8080_PREPROCESSOR_H
