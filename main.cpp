#include "Preprocessor.h"
#include "token_types.h"
#include "utility_functions.h"

#include <iostream>
#include <cstring>

int main(int argc, char *argv[]) {
    char filepath[]="hello_world.c";

    bool doRawLog = false;
    if (argc > 1) {
        if (strcmp(argv[1], "-log") == 0) {
            doRawLog = true;
        }
    }
    Scanner lexer(filepath, doRawLog);
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
