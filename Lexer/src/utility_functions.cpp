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
        case TokenType::OPEN_BRACE:             return "{";
        case TokenType::CLOSE_BRACE:            return "}";
        case TokenType::OPEN_BRACKET:           return "[";
        case TokenType::CLOSE_BRACKET:          return "]";
        case TokenType::OPEN_PARENS:            return "(";
        case TokenType::CLOSE_PARENS:           return ")";
        case TokenType::DOT:                    return ".";
        case TokenType::SELECTION:              return "->";
        case TokenType::PLUS_PLUS:              return "++";
        case TokenType::MINUS_MINUS:            return "--";
        case TokenType::AND:                    return "&";
        case TokenType::STAR:                   return "*";
        case TokenType::PLUS:                   return "+";
        case TokenType::MINUS:                  return "-";
        case TokenType::TILDE:                  return "~";
        case TokenType::NOT:                    return "!";
        case TokenType::SLASH:                  return "/";
        case TokenType::PERCENT:                return "%";
        case TokenType::LESS_LESS:              return "<<";
        case TokenType::GREATER_GREATER:        return ">>";
        case TokenType::LESS_THAN:              return "<";
        case TokenType::GREATER_THAN:           return ">";
        case TokenType::LESS_EQUAL:             return "<=";
        case TokenType::GREATER_EQUAL:          return ">=";
        case TokenType::EQUAL_EQUAL:            return "==";
        case TokenType::NOT_EQUAL:              return "!=";
        case TokenType::XOR:                    return "^";
        case TokenType::OR:                     return "|";
        case TokenType::AND_AND:                return "";
        case TokenType::OR_OR:                  return "";
        case TokenType::EQUAL:                  return "=";
        case TokenType::STAR_EQUAL:             return "*=";
        case TokenType::SLASH_EQUAL:            return "/=";
        case TokenType::PERCENT_EQUAL:          return "%=";
        case TokenType::PLUS_EQUAL:             return "+=";
        case TokenType::MINUS_EQUAL:            return "-=";
        case TokenType::LESS_LESS_EQUAL:        return "<<=";
        case TokenType::GREATER_GREATER_EQUAL:   return ">>=";
        case TokenType::AND_EQUAL:              return "=";
        case TokenType::XOR_EQUAL:              return "^=";
        case TokenType::OR_EQUAL:               return "|=";
        case TokenType::COMMA:                  return ",";
        case TokenType::HASH:                   return "#";
        case TokenType::HASH_HASH:              return "##";
        case TokenType::SEMICOLON:              return ";";
        case TokenType::COLON:                  return ":";
        case TokenType::ELLIPSIS:               return "...";

        case TokenType::UNKNOWN:              return "UNKNOWN TOKEN";

        default: return "token not added in getEnumString";
    }
}


bool isSymbol(const char& c)
{
    return c == '!' ||
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
        c == '#' ||
        c == '?';
};

bool isWhiteSpace(const char& c)
{
    if (c == '\t' || c == ' ') {
        return true;
    }
    return false;
}

bool checkSetSymbol(Token& token)
{
    if (token.value == "{")
        token.type = TokenType::OPEN_BRACE;
    else if (token.value == "}")
        token.type = TokenType::CLOSE_BRACE;
    else if (token.value == "[")
        token.type = TokenType::OPEN_BRACKET;
    else if (token.value == "]")
        token.type = TokenType::CLOSE_BRACKET;
    else if (token.value == "(")
        token.type = TokenType::OPEN_PARENS;
    else if (token.value == ")")
        token.type = TokenType::CLOSE_PARENS;
    else if (token.value == ".")
        token.type = TokenType::DOT;
    else if (token.value == "->")
        token.type = TokenType::SELECTION;
    else if (token.value == "++")
        token.type = TokenType::PLUS_PLUS;
    else if (token.value == "--")
        token.type = TokenType::MINUS_MINUS;
    else if (token.value == "&")
        token.type = TokenType::AND;
    else if (token.value == "*")
        token.type = TokenType::STAR;
    else if (token.value == "+")
        token.type = TokenType::PLUS;
    else if (token.value == "-")
        token.type = TokenType::MINUS;
    else if (token.value == "~")
        token.type = TokenType::TILDE;
    else if (token.value == "!")
        token.type = TokenType::NOT;
    else if (token.value == "/")
        token.type = TokenType::SLASH;
    else if (token.value == "%")
        token.type = TokenType::PERCENT;
    else if (token.value == "<<")
        token.type = TokenType::LESS_LESS;
    else if (token.value == ">>")
        token.type = TokenType::GREATER_GREATER;
    else if (token.value == "<")
        token.type = TokenType::LESS_THAN;
    else if (token.value == ">")
        token.type = TokenType::GREATER_THAN;
    else if (token.value == "<=")
        token.type = TokenType::LESS_EQUAL;
    else if (token.value == ">=")
        token.type = TokenType::GREATER_EQUAL;
    else if (token.value == "==")
        token.type = TokenType::EQUAL_EQUAL;
    else if (token.value == "!=")
        token.type = TokenType::NOT_EQUAL;
    else if (token.value == "^")
        token.type = TokenType::XOR;
    else if (token.value == "|")
        token.type = TokenType::OR;
    else if (token.value == "&&")
        token.type = TokenType::AND_AND;
    else if (token.value == "||")
        token.type = TokenType::OR_OR;
    else if (token.value == "=")
        token.type = TokenType::EQUAL;
    else if (token.value == "*=")
        token.type = TokenType::STAR_EQUAL;
    else if (token.value == "/=")
        token.type = TokenType::SLASH_EQUAL;
    else if (token.value == "%=")
        token.type = TokenType::PERCENT_EQUAL;
    else if (token.value == "+=")
        token.type = TokenType::PLUS_EQUAL;
    else if (token.value == "-=")
        token.type = TokenType::MINUS_EQUAL;
    else if (token.value == "<<=")
        token.type = TokenType::LESS_LESS_EQUAL;
    else if (token.value == ">>=")
        token.type = TokenType::GREATER_GREATER_EQUAL;
    else if (token.value == "&=")
        token.type = TokenType::AND_EQUAL;
    else if (token.value == "^=")
        token.type = TokenType::XOR_EQUAL;
    else if (token.value == "|=")
        token.type = TokenType::OR_EQUAL;
    else if (token.value == ",")
        token.type = TokenType::COMMA;
    else if (token.value == "#")
        token.type = TokenType::HASH;
    else if (token.value == "##")
        token.type = TokenType::HASH_HASH;
    else if (token.value == ";")
        token.type = TokenType::SEMICOLON;
    else if (token.value == ":")
        token.type = TokenType::COLON;
    else if (token.value == "...")
        token.type = TokenType::ELLIPSIS;

    if (token.type == TokenType::UNKNOWN)
        return false;
    return false;
};

void logTokenCreation(const Token& token, const int& depth) {
    //assert(token.type != TokenType::UNKNOWN);
    for(int i = 0; i < depth; i++){
        std::cout << " ";
    }
    std::cout << getPrintableTokenValue(token) << " add as " << getEnumString(token.type) << std::endl;
}

std::string getPrintableTokenValue(const Token token)
{
    if (token.value == "\n")
        return "\\n";
    else if (token.value == "\t")
        return "\\t";
    else if (token.type == TokenType::WS)
        return "' '";
    else if (token.value.at(0) == EOF)
        return "EOF";
    else
        return token.value;
}