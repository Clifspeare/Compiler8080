//
// Created by spencer on 3/2/18.
//

#ifndef COMPILER8080_TOKEN_TYPES_H
#define COMPILER8080_TOKEN_TYPES_H
#include <string>

enum class TokenType
{
    PREPROCESSOR_SYMBOL,
    DEFINE,
    INCLUDE,
    WS,
    INT,
    FLOAT,
    DOUBLE,
    ID,
    CONSTANT,
    LITERAL,
    LESS_THAN
};

struct token {
    TokenType type;
    std::string* value;
};

#endif //COMPILER8080_TOKEN_TYPES_H
