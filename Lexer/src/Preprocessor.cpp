//
// Created by spencer on 3/4/18.
//

#include "../include/Preprocessor.h"
#include <cassert>

Token Preprocessor::getNextToken() {
  Token token = m_scanner.getNextToken();

  if (token.type != TokenType::HASH) {
    if (token.type == TokenType::ID) {
      if (hasDefinition(token)) {
        token = getDefinitionReplacement(token.value);
      }
    }
    return token;
  } else {  // if token is #
    // Handle #include and define.
    if ((token = m_scanner.getNextToken()).type == TokenType::ID) {
      // Handle #include.
      if (token.value == "include") {
        token = m_scanner.getNextToken();
        if (token.type == TokenType::LITERAL) {
          m_scanner.loadSourceFile(token.value.c_str());
        } else if (token.type == TokenType::LESS_THAN) {
          std::string filename;
          std::string dot;
          std::string ext;
          // TODO: make system headers work
        } else {
          std::cout << "ERROR" << std::endl;
        }
        token = getNextToken();
      }
      // Handle #define.
      else if (token.value == "define") {
        token = m_scanner.getNextToken();
        if (token.type == TokenType::ID) {
          Token destToken = m_scanner.getNextToken();
          addDefinition(token, destToken);
          token = getNextToken();
        }
      }
    }
    return token;
  }
}

// TODO: Add error handling for TokenType values.
void Preprocessor::addDefinition(Token definition, Token value) {
  // also check TokenType of value
  assert(definition.type == TokenType::ID);

  Definition def = {definition.value, value};
  m_defines.push_back(def);
}

bool Preprocessor::hasDefinition(Token token) {
  for (Definition def : m_defines) {
    if (def.definition == token.value) {
      return true;
    }
  }
  return false;
}

Token Preprocessor::getDefinitionReplacement(std::string idStringToBeReplaced) {
  for (Definition defInstance : m_defines) {
    if (defInstance.definition == idStringToBeReplaced) {
      return defInstance.token;
    }
  }

  Token failed;
  failed.type = TokenType::UNKNOWN;

  return failed;
}

Preprocessor::Preprocessor(char *filepath) : m_scanner(filepath) {}

Preprocessor::Preprocessor(Scanner existingScanner)
    : m_scanner(existingScanner) {}
