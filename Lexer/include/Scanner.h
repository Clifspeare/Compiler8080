#ifndef COMPILER8080_SCANNER_H
#define COMPILER8080_SCANNER_H

#include "token_types.h"
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include <array>

// Need to write fstream that reads in spaces and new lines in getNextToken.

struct source_file {
    std::ifstream* i_stream;
    int index;
};

class Scanner {
public:
    Scanner();
    Scanner(char* filename);

    // Pushes new source file onto the stack.
    void loadSourceFile(char* filename);

    token getNextToken();
private:
    // implementation of token creation (from file) with state transitions, etc.

    std::vector<token> m_tokens;
    std::vector<std::string> m_literals;
    std::vector<std::string> m_constants;
    std::vector<std::string> m_identifiers;
    std::vector<std::string> m_reserved;

    std::ifstream* m_current_file;
    std::stack<source_file> m_source_files;
};


#endif //COMPILER8080_SCANNER_H
