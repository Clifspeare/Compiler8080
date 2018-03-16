#include "Preprocessor.h"
#include "token_types.h"
#include "utility_functions.h"

#include <iostream>

int main() {
    char filepath[]="hello_world.c";
    Preprocessor lexer(filepath);
    Token token;
    int counter = 0;
    while ((token = lexer.getNextToken()).value != "eof") {
        if(token.value == "end")
            break;
    }
    std::cout << "end reached";
    return 0;
}
