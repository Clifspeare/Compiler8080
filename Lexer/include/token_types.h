//
// Created by spencer on 3/2/18.
//

#ifndef COMPILER8080_TOKEN_TYPES_H
#define COMPILER8080_TOKEN_TYPES_H
#include <string>
#include <map>

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
    GREATER_THAN,

    // reserved words
    //native types
    INT,
    FLOAT,
    DOUBLE,
    BOOL,
    CHAR,
    SHORT,
    LONG,
    VOID,
    //keywords
    IF,
    ELSE,
    WHILE,
    FOR,
    DO,
    SWITCH,
    CASE,
    CONTINUE,
    BREAK,
    STRUCT,
    RETURN,
    VOLATILE,
    ENUM,
    EXTERN,
    STATIC,
    CONST,
    AUTO,
    TYPEDEF,
    DEFAULT,
    REGISTER,
    UNION,
    SIGNED,
    UNSIGNED,
    GOTO,

    UNDEFINED
};

struct Token {
    TokenType type;
    std::string value;
};

#endif //COMPILER8080_TOKEN_TYPES_H
