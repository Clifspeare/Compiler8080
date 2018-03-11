//
// Created by zzaz3 on 3/11/2018.
//

#ifndef COMPILER8080_UTILITY_FUNCTIONS_H
#define COMPILER8080_UTILITY_FUNCTIONS_H


#include <assert.h>
#include <iostream>
#include "token_types.h"

std::string getEnumString(TokenType enumval);

bool isSymbol(const char& c);
bool isWhiteSpace(const char& c);

// Error handling.
void logTokenCreation(const Token& token);

#endif //COMPILER8080_UTILITY_FUNCTIONS_H
