//
// Created by spencer on 3/4/18.
//

#include <cassert>
#include "../include/Preprocessor.h"

Token Preprocessor::getNextToken() {
    Token token = m_scanner.getNextToken();
    if (token.type != TokenType::PREPROCESSOR_SYMBOL) {
        // Iterate through def table and replace value if found.
        if (token.type == TokenType::ID) {
            for (const Definition def : m_defines) {
                if (def.token.value == token.value) {
                    token.value = def.token.value;
                    token.type = def.token.type;
                    return token;
                }
            }
        }
        // Else.
        return token;
    }

    token = m_scanner.getNextToken();
    if (token.type == TokenType::DEFINE) {
        addDefinition(m_scanner.getNextToken(), m_scanner.getNextToken());
    } else if (token.type == TokenType::INCLUDE) {
        //TODO: push new source file onto Scanner and get all tokens from that
    } else { // nothing for now

    }
}

// TODO: Add error handling for TokenType values.
void Preprocessor::addDefinition(Token definition, Token value) {
    //also check TokenType of value
    assert(definition.type == TokenType::ID);

    Definition def = {definition.value, value};
    m_defines.push_back(def);
}

