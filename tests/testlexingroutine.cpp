#include "Preprocessor.h"
#include "token_types.h"
#include "utility_functions.h"

#include <iostream>
#include <cstring>

int main(int argc, char *argv[]) {
    char filepath[]="input_file.c";

    bool doRawLog = false;
    bool print = true;
    if (argc > 1) {
        if (strcmp(argv[1], "-log") == 0) {
            doRawLog = true;
        }
        if (strcmp(argv[1], "-print") == 0) {
            print = true;
        }
    }
    Scanner lexer(filepath, doRawLog);
    Preprocessor preprocessor(lexer);
    Token token;
    int counter = 0;
    token = preprocessor.getNextToken();
    while (token.value != "end") {
        if (print)
            std::cout << token.value << std::endl;
        token = preprocessor.getNextToken();
    }
    std::cout << "end reached" << std::endl;
    return 0;
}
