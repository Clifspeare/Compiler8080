//
// Created by spencer on 3/4/18.
//

#ifndef COMPILER8080_PREPROCESSOR_H
#define COMPILER8080_PREPROCESSOR_H
#include "Scanner.h"
#include "token_types.h"
#include <algorithm>

struct definition {
    std::string definition;
    token token;
};



class Preprocessor {
public:
    Preprocessor();

    token getNextToken();
private:
    void addDefinition(token definition, token value);

    Scanner m_scanner;

    std::vector<definition> m_defines;
};


#endif //COMPILER8080_PREPROCESSOR_H
