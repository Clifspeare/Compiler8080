//
// Created by spencer on 3/2/18.
//

#ifndef COMPILER8080_TOKEN_TYPES_H
#define COMPILER8080_TOKEN_TYPES_H
#include <map>
#include <string>

enum class TokenType {
  ID,
  CONSTANT,
  FLOATING_CONSTANT,
  CHARACTER_CONSTANT,
  ENUMERATION_CONSTANT,
  LITERAL,
  COMMENT,
  // preprocessors
  PREPROCESSOR_SYMBOL,
  DEFINE,
  INCLUDE,

  // special
  LF,
  WS,
  OPEN_BRACE,
  CLOSE_BRACE,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  OPEN_PARENS,
  CLOSE_PARENS,
  DOT,
  SELECTION,
  PLUS_PLUS,
  MINUS_MINUS,
  AND,
  STAR,
  PLUS,
  MINUS,
  TILDE,
  NOT,
  SLASH,
  PERCENT,
  LESS_LESS,
  GREATER_GREATER,
  LESS_THAN,
  GREATER_THAN,
  LESS_EQUAL,
  GREATER_EQUAL,
  EQUAL_EQUAL,
  NOT_EQUAL,
  XOR,
  OR,
  AND_AND,
  OR_OR,
  EQUAL,
  STAR_EQUAL,
  SLASH_EQUAL,
  PERCENT_EQUAL,
  PLUS_EQUAL,
  MINUS_EQUAL,
  LESS_LESS_EQUAL,
  GREATER_GREATER_EQUAL,
  AND_EQUAL,
  XOR_EQUAL,
  OR_EQUAL,
  COMMA,
  HASH,
  HASH_HASH,
  SEMICOLON,
  COLON,
  ELLIPSIS,
  QUESTION_MARK,
  // reserved words
  // native types
  INT,
  FLOAT,
  DOUBLE,
  BOOL,
  CHAR,
  SHORT,
  LONG,
  VOID,
  // keywords
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
  SIZEOF,

  UNKNOWN
};

struct Token {
  TokenType type = TokenType::UNKNOWN;
  std::string value;
};

#endif  // COMPILER8080_TOKEN_TYPES_H
