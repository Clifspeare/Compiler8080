#include "Preprocessor.h"
#include "token_types.h"
#include "utility_functions.h"

#include <iostream>

int main() {
    char filepath[]="hello_world.c";
    Preprocessor lexer(filepath);
    Token token;
    int counter = 0;
    while ((token = lexer.getNextToken()).value != "end") {
    }
    std::cout << "end reached" << std::endl;
    return 0;
}
