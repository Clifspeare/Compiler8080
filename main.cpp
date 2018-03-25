#include "Preprocessor.h"
#include "token_types.h"
#include "utility_functions.h"

#include <iostream>

int main() {
    char filepath[]="hello_world.c";
    Preprocessor lexer(filepath);
    Token token;
    int counter = 0;
    token = lexer.getNextToken();
    while (token.value != "end") {
        token = lexer.getNextToken();
    }
    std::cout << "end reached" << std::endl;
    return 0;
}
