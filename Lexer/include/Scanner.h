#ifndef COMPILER8080_SCANNER_H
#define COMPILER8080_SCANNER_H

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include "token_types.h"
#include "utility_functions.h"

class Scanner {
 public:
  Scanner();
  Scanner(char* filename);
  Scanner(char* filename, bool doRawTokenLogging);

  // Push and pop functions manage state of the open source files.
  void loadSourceFile(const char* filepath);
  void popSourceFile();

  Token getNextToken();

 private:
  // implementation of token creation (from file) with state transitions, etc.
  bool checkSetReservedWord(Token& token);

  char getNextChar();
  void unGetChar(char Character);
  int depth = 0;  // Count of how many files are open.
  bool end = false;
  bool logRawTokens = false;

  std::vector<Token> m_tokens;
  std::vector<std::string> m_literals;
  std::vector<std::string> m_constants;
  std::vector<std::string> m_identifiers;
  std::map<std::string, TokenType> m_reservedWords = {
      std::make_pair("int", TokenType::INT),
      std::make_pair("double", TokenType::DOUBLE),
      std::make_pair("float", TokenType::FLOAT),
      std::make_pair("short", TokenType::SHORT),
      std::make_pair("long", TokenType::LONG),
      std::make_pair("char", TokenType::CHAR),
      std::make_pair("bool", TokenType::BOOL),
      std::make_pair("void", TokenType::VOID),
      std::make_pair("if", TokenType::IF),
      std::make_pair("else", TokenType::ELSE),
      std::make_pair("return", TokenType::RETURN),
      std::make_pair("struct", TokenType::STRUCT),
      std::make_pair("while", TokenType::WHILE),
      std::make_pair("for", TokenType::FOR),
      std::make_pair("volatile", TokenType::VOLATILE),
      std::make_pair("enum", TokenType::ENUM),
      std::make_pair("extern", TokenType::EXTERN),
      std::make_pair("static", TokenType::STATIC),
      std::make_pair("continue", TokenType::CONTINUE),
      std::make_pair("const", TokenType::CONST),
      std::make_pair("auto", TokenType::AUTO),
      std::make_pair("typedef", TokenType::TYPEDEF),
      std::make_pair("default", TokenType::DEFAULT),
      std::make_pair("register", TokenType::REGISTER),
      std::make_pair("switch", TokenType::SWITCH),
      std::make_pair("case", TokenType::CASE),
      std::make_pair("union", TokenType::UNION),
      std::make_pair("signed", TokenType::SIGNED),
      std::make_pair("unsigned", TokenType::UNSIGNED),
      std::make_pair("do", TokenType::DO),
      std::make_pair("break", TokenType::BREAK),
      std::make_pair("goto", TokenType::GOTO)};

  std::shared_ptr<std::ifstream> m_current_file;
  std::stack<std::shared_ptr<std::ifstream>> m_source_files;
};

#endif  // COMPILER8080_SCANNER_H
