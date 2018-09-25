//
// Created by spencer on 3/4/18.
//

#ifndef COMPILER8080_PREPROCESSOR_H
#define COMPILER8080_PREPROCESSOR_H
#include <algorithm>
#include "Scanner.h"

struct Definition {
  std::string definition;
  Token token;
};

class Preprocessor {
 public:
  Preprocessor(char* filepath);
  Preprocessor(Scanner existingScanner);

  Token getNextToken();

 private:
  void addDefinition(Token definition, Token value);
  bool hasDefinition(Token token);
  Token getDefinitionReplacement(std::string idStringToBeReplaced);

  Scanner m_scanner;

  std::vector<Definition> m_defines;
};

#endif  // COMPILER8080_PREPROCESSOR_H
