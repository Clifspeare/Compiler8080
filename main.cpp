#include "Scanner.h"
#include "token_types.h"
#include "utility_functions.h"

#include <iostream>

int main() {
    char filepath[]="hello_world.c";
    Scanner lexer(filepath);
    Token token;
    int counter = 0;
    while ((token = lexer.getNextToken()).type != TokenType::UNDEFINED) {
        // std::cout << "Token" << counter++ << " " << getEnumString(token.type) << std::endl; Already logged.
    }
    return 0;
}
