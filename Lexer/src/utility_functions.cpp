//
// Created by zzaz3 on 3/11/2018.
//

#include "../include/utility_functions.h"

std::string getEnumString(TokenType enumval)
{
    switch (enumval) {
        case TokenType::INT:                    return "int";
        case TokenType::FLOAT:                  return "float";
        case TokenType::DOUBLE:                 return "double";
        case TokenType::BOOL:                   return "bool";
        case TokenType::CHAR:                   return "char";
        case TokenType::SHORT:                  return "short";
        case TokenType::LONG:                   return "long";
        case TokenType::VOID:                   return "void";

        case TokenType::IF:                     return "if";
        case TokenType::ELSE:                   return "else";
        case TokenType::WHILE:                  return "while";
        case TokenType::FOR:                    return "for";
        case TokenType::DO:                     return "do";
        case TokenType::SWITCH:                 return "switch";
        case TokenType::CASE:                   return "case";
        case TokenType::CONTINUE:               return "continue";
        case TokenType::BREAK:                  return "break";
        case TokenType::STRUCT:                 return "struc";
        case TokenType::RETURN:                 return "return";
        case TokenType::VOLATILE:               return "volatile";
        case TokenType::ENUM:                   return "enum";
        case TokenType::EXTERN:                 return "extern";
        case TokenType::STATIC:                 return "static";
        case TokenType::CONST:                  return "const";
        case TokenType::AUTO:                   return "auto";
        case TokenType::TYPEDEF:                return "typedef";
        case TokenType::DEFAULT:                return "default";
        case TokenType::REGISTER:               return "register";
        case TokenType::UNION:                  return "union";
        case TokenType::SIGNED:                 return "signed";
        case TokenType::UNSIGNED:               return "unsigned";
        case TokenType::GOTO:                   return "goto";

        case TokenType::ID:                     return "id";
        case TokenType::CONSTANT:               return "constant";
        case TokenType::LITERAL:                return "literal";
        case TokenType::PREPROCESSOR_SYMBOL:    return "#";
        case TokenType::DEFINE:                 return "define";
        case TokenType::INCLUDE:                return "include";
        case TokenType::LF:                     return "\\n";
        case TokenType::WS:                     return "white space";
        case TokenType::LESS_THAN:              return "<";
        case TokenType::GREATER_THAN:           return ">";

        case TokenType::UNDEFINED:              return "UNDEFINED TOKEN";

        default: return "token not added in getEnumString";
    }
}


bool isSymbol(const char& c)
{
    if (
            c == '!' ||
            c == '{' ||
            c == '}' ||
            c == '(' ||
            c == ')' ||
            c == '[' ||
            c == ']' ||
            c == '+' ||
            c == '-' ||
            c == '%' ||
            c == '/' ||
            c == '*' ||
            c == '&' ||
            c == '<' ||
            c == '>' ||
            c == '=' ||
            c == '^' ||
            c == '|' ||
            c == '~' ||
            c == ';' ||
            c == ',' ||
            c == '.' ||
            c == ':' ||
            c == '?'
            )
    {
        return true;
    }
    return false;
};

bool isWhiteSpace(const char& c)
{
    if (c == '\t' || c == ' ') {
        return true;
    }
}

void logTokenCreation(const Token& token) {
    assert(token.type != TokenType::UNDEFINED);
    std::cout << token.value << " add as " << getEnumString(token.type) << std::endl;
}