//
// Created by spencer on 3/2/18.
//

#ifndef COMPILER8080_TOKEN_TYPES_H
#define COMPILER8080_TOKEN_TYPES_H
#include <string>

enum class TokenType
{
    ID,
    CONSTANT,
    LITERAL,
    // preprocessors
    PREPROCESSOR_SYMBOL,
    DEFINE,
    INCLUDE,

    // special
    LF,
    WS,
    LESS_THAN,

    // reserved words
    INT,
    FLOAT,
    DOUBLE,

    UNDEFINED
};

struct Token {
    TokenType type;
    std::string value;
};

#endif //COMPILER8080_TOKEN_TYPES_H
