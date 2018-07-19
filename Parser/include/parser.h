#ifndef PARSER_H
#define PARSER_H

#include "Preprocessor.h"
#include "ast_type.h"
#include "node.h"
#include "token_types.h"

template <class T>
class SymbolEntry {
 public:
  SymbolEntry(T new_symbol) { symbol_m = new_symbol; }

  std::string identifier_name_m;
  T symbol_m;
  SymbolEntry* next_m;
};

class Parser {
 public:
  Parser();

  bool HasTokens();

 private:
  Preprocessor m_preprocessor;
  std::unique_ptr<Node> m_root;
  std::vector<std::string> m_errors;

  // TODO: make this better.
  Token getNextToken();
  int m_tokenIndex = 0;
  std::vector<Token> m_tokens;

  // Recursive-descent non-terminal parsing routines
  std::unique_ptr<Node> translation_unit();
  std::unique_ptr<Node> external_declaration();
  std::unique_ptr<Node> function_definition();
  std::unique_ptr<Node> declaration_specifier();
  std::unique_ptr<Node> storage_class_specifier();
  std::unique_ptr<Node> type_specifier();
  std::unique_ptr<Node> struct_or_union_specifer();
  std::unique_ptr<Node> struct_or_union();
  std::unique_ptr<Node> struct_declaration();
  std::unique_ptr<Node> specifier_qualifier();
  std::unique_ptr<Node> struct_declarator_list();
  std::unique_ptr<Node> struct_declarator();
  std::unique_ptr<Node> declarator();
  std::unique_ptr<Node> pointer();
  std::unique_ptr<Node> type_qualifier();
  std::unique_ptr<Node> direct_declarator();
  std::unique_ptr<Node> constant_expression();
  std::unique_ptr<Node> logical_or_expression();
  std::unique_ptr<Node> logical_and_expression();
  std::unique_ptr<Node> inclusive_or_expression();
  std::unique_ptr<Node> and_expression();
  std::unique_ptr<Node> conditional_expression();
  std::unique_ptr<Node> equality_expression();
  std::unique_ptr<Node> relational_expression();
  std::unique_ptr<Node> shift_expression();
  std::unique_ptr<Node> additive_expression();
  std::unique_ptr<Node> multiplicative_expression();
  std::unique_ptr<Node> cast_expression();
  std::unique_ptr<Node> unary_expression();
  std::unique_ptr<Node> postfix_expression();
  std::unique_ptr<Node> primary_expression();
  std::unique_ptr<Node> constant();
  std::unique_ptr<Node> expression();
  std::unique_ptr<Node> assignment_expression();
  std::unique_ptr<Node> assignment_operator();
  std::unique_ptr<Node> unary_operator();
  std::unique_ptr<Node> type_name();
  std::unique_ptr<Node> parameter_type_list();
  std::unique_ptr<Node> parameter_list();
  std::unique_ptr<Node> parameter_declaration();
  std::unique_ptr<Node> abstract_declarator();
  std::unique_ptr<Node> direct_abstract_declarator();
  std::unique_ptr<Node> enum_specifer();
  std::unique_ptr<Node> enumerator_list();
  std::unique_ptr<Node> enumerator();
  std::unique_ptr<Node> typedef_name();
  std::unique_ptr<Node> declaration();
  std::unique_ptr<Node> init_declarator();
  std::unique_ptr<Node> initializer();
  std::unique_ptr<Node> initializer_list();
  std::unique_ptr<Node> compound_statement();
  std::unique_ptr<Node> statement();
  std::unique_ptr<Node> labeled_statement();
  std::unique_ptr<Node> expression_statement();
  std::unique_ptr<Node> selection_statement();
  std::unique_ptr<Node> iteration_statement();
  std::unique_ptr<Node> jump_statement();
};

#endif