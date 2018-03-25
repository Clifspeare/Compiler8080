#include "Preprocessor.h"
#include "token_types.h"
#include "utility_functions.h"

#include <iostream>

int main() {
    char filepath[]="hello_world.c";
    Scanner lexer(filepath);
    Preprocessor preprocessor(lexer);
    Token token;
    int counter = 0;
    token = preprocessor.getNextToken();
    while (token.value != "end") {
        token = preprocessor.getNextToken();
    }
    std::cout << "end reached" << std::endl;
    return 0;
}
