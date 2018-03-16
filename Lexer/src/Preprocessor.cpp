//
// Created by spencer on 3/4/18.
//

#include <cassert>
#include "../include/Preprocessor.h"

Token Preprocessor::getNextToken() {
    Token token = m_scanner.getNextToken();
    if (token.type == TokenType::UNKNOWN) {
        m_scanner.popSourceFile();
    }
    if (token.type != TokenType::HASH) {
        return token;
    } else {
        if ((token = m_scanner.getNextToken()).type == TokenType::ID) {

            // handle defines and includes
            if (token.value == "include") {
                token = m_scanner.getNextToken();
                if (token.type != TokenType::WS)
                    std::cout << "ERROR" << std::endl;
                token = m_scanner.getNextToken();
                if (token.type == TokenType::LITERAL) {
                    m_scanner.loadSourceFile(token.value.c_str());
                } else if (token.type == TokenType::LESS_THAN) {
                    std::string filename;
                    std::string dot;
                    std::string ext;
                } else {
                    std::cout << "ERROR" << std::endl;
                }
                return m_scanner.getNextToken();
            }
        }
        return token;
    }
}

// TODO: Add error handling for TokenType values.
void Preprocessor::addDefinition(Token definition, Token value) {
    //also check TokenType of value
    assert(definition.type == TokenType::ID);

    Definition def = {definition.value, value};
    m_defines.push_back(def);
}

Preprocessor::Preprocessor(char *filepath) : m_scanner(filepath) {
}

