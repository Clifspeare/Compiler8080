#ifndef PARSER_H
#define PARSER_H

#include <functional>

#include "Preprocessor.h"
#include "node.h"

#define DEBUG 1

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

  std::shared_ptr<Node> getTranslationUnit() {return m_root;};

  bool HasTokens();

 private:
  Preprocessor m_preprocessor;
  std::shared_ptr<Node> m_root;
  std::vector<std::string> m_errors;

  // TODO: make this better.
  Token getNextToken();
  int m_tokenIndex = 0;
  std::vector<Token> m_tokens;

  int computeTreeSize(std::shared_ptr<Node> node);
  bool callNonterminalProcedure(std::shared_ptr<Node>(Parser::*fn)(), std::shared_ptr<Node> self, bool optional = false);
  bool HandleTerminal(TokenType token_type, Type node_type, std::shared_ptr<Node> self, bool mandatory = false);
  std::shared_ptr<Node> HandleUnexpectedTerminal(std::shared_ptr<Node> self);

  // TODO: add Type Registration for typedef

    // Recursive-descent non-terminal parsing routines
    std::shared_ptr<Node> translation_unit(); //
    std::shared_ptr<Node> external_declaration(); //
    std::shared_ptr<Node> function_definition(); //
    std::shared_ptr<Node> declaration_specifier();
    std::shared_ptr<Node> storage_class_specifier(); //
    std::shared_ptr<Node> type_specifier(); //
    std::shared_ptr<Node> struct_or_union_specifier();
    std::shared_ptr<Node> struct_or_union();
    std::shared_ptr<Node> struct_declaration();
    std::shared_ptr<Node> specifier_qualifier();
    std::shared_ptr<Node> struct_declarator_list();
    std::shared_ptr<Node> struct_declarator();
    std::shared_ptr<Node> declarator(); //
    std::shared_ptr<Node> pointer();
    std::shared_ptr<Node> type_qualifier();
    std::shared_ptr<Node> direct_declarator();
    std::shared_ptr<Node> constant_expression();
    std::shared_ptr<Node> logical_or_expression();
    std::shared_ptr<Node> logical_and_expression();
    std::shared_ptr<Node> inclusive_or_expression();
    std::shared_ptr<Node> exclusive_or_expression();
    std::shared_ptr<Node> and_expression();
    std::shared_ptr<Node> conditional_expression();
    std::shared_ptr<Node> equality_expression();
    std::shared_ptr<Node> relational_expression();
    std::shared_ptr<Node> shift_expression();
    std::shared_ptr<Node> additive_expression();
    std::shared_ptr<Node> multiplicative_expression();
    std::shared_ptr<Node> cast_expression();
    std::shared_ptr<Node> unary_expression();
    std::shared_ptr<Node> postfix_expression();
    std::shared_ptr<Node> primary_expression();
    std::shared_ptr<Node> constant();
    std::shared_ptr<Node> expression();
    std::shared_ptr<Node> assignment_expression();
    std::shared_ptr<Node> assignment_operator();
    std::shared_ptr<Node> unary_operator();
    std::shared_ptr<Node> type_name();
    std::shared_ptr<Node> parameter_type_list();
    std::shared_ptr<Node> parameter_list();
    std::shared_ptr<Node> parameter_declaration();
    std::shared_ptr<Node> abstract_declarator();
    std::shared_ptr<Node> direct_abstract_declarator();
    std::shared_ptr<Node> enum_specifier();
    std::shared_ptr<Node> enumerator_list();
    std::shared_ptr<Node> enumerator();
    std::shared_ptr<Node> typedef_name();
    std::shared_ptr<Node> declaration();
    std::shared_ptr<Node> init_declarator();
    std::shared_ptr<Node> initializer();
    std::shared_ptr<Node> initializer_list();
    std::shared_ptr<Node> compound_statement();
    std::shared_ptr<Node> statement(); //
    std::shared_ptr<Node> labeled_statement();
    std::shared_ptr<Node> expression_statement();
    std::shared_ptr<Node> selection_statement();
    std::shared_ptr<Node> iteration_statement();
    std::shared_ptr<Node> jump_statement(); //

    std::shared_ptr<Node> keyword_if();
};

#endif
