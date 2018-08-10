#include "parser.h"

#include <iostream>
#include <memory>

Parser::Parser() : m_preprocessor(), m_root(), m_errors(), m_tokens() {
  m_root = std::make_shared<Node>();
  Token tok = m_preprocessor.getNextToken();
  while (tok.value != "end" && tok.value != "eof") {
    m_tokens.push_back(tok);
    tok = m_preprocessor.getNextToken();
  }
}

bool Parser::HasTokens() {
  if (m_tokenIndex == m_tokens.size()) {
    return false;
  }

  return true;
}

Token Parser::getNextToken() {
  Token t = m_tokens.at(m_tokenIndex++);
  return t;
}

struct ErrorInfo {
  Type expected_type;
  std::string actual_value;
};

ErrorInfo getErrorInfo(std::shared_ptr<Node>& node) 
{
  if (node->type == Type::ERROR) {
    ErrorInfo err_no;
    err_no.expected_type = node->parent->type;
    err_no.actual_value = node->data;
    return err_no;
  }
  for (int i = 0; node->children.size(); ++i) {
    auto n = (node);
    if (n->accepted) {
      return getErrorInfo(n);
    }
  }
}

// PRINT DEBUG
void PrintErrorMessage(ErrorInfo info) 
{
  std::cout << "ERROR! AAH!" << std::endl;
}

// FILE's ENTRY POINT
std::shared_ptr<Node> Parser::translation_unit() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::TRANSLATION_UNIT;
  
  while (HasTokens()) {
    bool compilation_error = !(callNonterminalProcedure(Parser::external_declaration, self));

    if(compilation_error) {
      PrintErrorMessage(getErrorInfo(self));
    }
  }

  return self;
}

// EXTERNAL DECLARATION (FUNCTION OR GLOBAL VARIABLE)
std::shared_ptr<Node> Parser::external_declaration() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  
  if(callNonterminalProcedure(&Parser::function_definition, self)) {
    return self;
  }

  callNonterminalProcedure(&Parser::declaration, self);

  return self;
}

// FUNCTION DEFINITION
std::shared_ptr<Node> Parser::function_definition() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::FUNCTION_DEFINITION;

  while (callNonterminalProcedure(&Parser::declaration_specifier, self, true));
  
  if (!callNonterminalProcedure(&Parser::declarator, self))
    return self;

  while (callNonterminalProcedure(&Parser::declaration, self, true));

  if(!callNonterminalProcedure(&Parser::compound_statement, self));
    return self;

  return self;
}

//
std::shared_ptr<Node> Parser::declaration_specifier() 
{
  // TODO: talk to Zach about possibly altering declaration_specifier() grammar.

  // std::shared_ptr<Node> self = std::make_shared<Node>();
  // self->type = Type::DECLARATION_SPECIFIER;

  // std::shared_ptr<Node> storage_class_specifier_node;
  // std::shared_ptr<Node> type_specifier_node;
  // std::shared_ptr<Node> type_qualifier_node;

  // int saved_token_index = m_tokenIndex;
  // storage_class_specifier_node

  // return self;
}

// TODO: Segregate the terminals and non-terminals.

std::shared_ptr<Node> Parser::storage_class_specifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STORAGE_CLASS_SPECIFIER;

  bool terminal_rejected = false;

  terminal_rejected = !(HandleTerminal(TokenType::AUTO, Type::AUTO, self)
                     || HandleTerminal(TokenType::REGISTER, Type::REGISTER, self)
                     || HandleTerminal(TokenType::EXTERN, Type::EXTERN, self)
                     || HandleTerminal(TokenType::TYPEDEF, Type::TYPEDEF, self));
                   
  if (terminal_rejected) {
    HandleUnexpectedTerminal(self);
  }

  return self;
}

std::shared_ptr<Node> Parser::type_specifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::TYPE_SPECIFIER;

  bool terminal_rejected = !(HandleTerminal(TokenType::VOID, Type::VOID, self)
                          || HandleTerminal(TokenType::CHAR, Type::CHAR, self)
                          || HandleTerminal(TokenType::SHORT, Type::SHORT, self)
                          || HandleTerminal(TokenType::INT, Type::INT, self)
                          || HandleTerminal(TokenType::LONG, Type::LONG, self)
                          || HandleTerminal(TokenType::FLOAT, Type::FLOAT, self)
                          || HandleTerminal(TokenType::DOUBLE, Type::DOUBLE, self)
                          || HandleTerminal(TokenType::SIGNED, Type::SIGNED, self)
                          || HandleTerminal(TokenType::UNSIGNED, Type::UNSIGNED, self));

  callNonterminalProcedure(&Parser::struct_or_union_specifier, self, true);
  
  callNonterminalProcedure(&Parser::enum_specifier, self, true);
  
  callNonterminalProcedure(&Parser::typedef_name, self, true);

  if (terminal_rejected) {
    HandleUnexpectedTerminal(self);
  } // handle case where it should be one of the nonterminals and they were partially successful and contain error?

  return self;
}

std::shared_ptr<Node> Parser::struct_or_union_specifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STRUCT_OR_UNION_SPECIFIER;

  callNonterminalProcedure(&Parser::struct_or_union, self);
  HandleTerminal(TokenType::ID, Type::IDENTIFIER, self);

  if (HandleTerminal(TokenType::OPEN_BRACKET, Type::OPEN_BRACKET, self)) {
    while(callNonterminalProcedure(&Parser::struct_declaration, self));
    HandleTerminal(TokenType::CLOSE_BRACKET, Type::CLOSE_BRACKET, self, true);
  }

  return self;
}

std::shared_ptr<Node> Parser::struct_or_union() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STRUCT_OR_UNION;

  bool terminal_rejected = !(HandleTerminal(TokenType::STRUCT, Type::STRUCT, self)
                        ||   HandleTerminal(TokenType::UNION, Type::UNION, self));
  if (terminal_rejected) {
    HandleUnexpectedTerminal(self);
  }

  return self;
}

std::shared_ptr<Node> Parser::struct_declaration()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STRUCT_DECLARATION;

  while (callNonterminalProcedure(&Parser::specifier_qualifier, self, true));

  callNonterminalProcedure(&Parser::struct_declarator_list, self);

  return self;
}

std::shared_ptr<Node> Parser::specifier_qualifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STRUCT_DECLARATION;

  bool rejected_nonterminal = !(callNonterminalProcedure(&Parser::type_specifier, self, true)
                            || callNonterminalProcedure(&Parser::type_qualifier, self, true));
  if (rejected_nonterminal) {
    // unexpected terminal where nonterminal is expected TODO: handle gracefully
  }
  return self;
}

std::shared_ptr<Node> Parser::struct_declarator_list()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STRUCT_DECLARATOR_LIST;

  while(callNonterminalProcedure(&Parser::struct_declarator, self)) {
    if (!HandleTerminal(TokenType::COMMA, Type::COMMA, self)) {
      break; 
    };
  }

  return self; 
}

std::shared_ptr<Node> Parser::struct_declarator() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STRUCT_DECLARATOR;

  callNonterminalProcedure(&Parser::declarator, self, true);
  if (HandleTerminal(TokenType::COLON, Type::COLON, self)) {
    callNonterminalProcedure(&Parser::constant_expression, self)
  }
  return self;
}

//
std::shared_ptr<Node> Parser::declarator() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::DECLARATOR;

  callNonterminalProcedure(&Parser::pointer, self, true);
  callNonterminalProcedure(&Parser::direct_declarator, self);

  return self;
}
std::shared_ptr<Node> Parser::pointer() //TODO: this one hurts me.  maybe overthinking.  might as well leave it to Monday.
{
}
std::shared_ptr<Node> Parser::type_qualifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::TYPE_QUALIFIER;

  auto terminal_rejected = !(HandleTerminal(TokenType::CONST, Type::CONST_KEYWORD, self)
                        || HandleTerminal(TokenType::VOLATILE, Type::VOLATILE_KEYWORD, self));
  if (terminal_rejected) {
    HandleUnexpectedTerminal(self);
  }

  return self;
}

//TODO left-recursive
std::shared_ptr<Node> Parser::direct_declarator() 
{

}
std::shared_ptr<Node> Parser::constant_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::CONSTANT_EXPRESSION;

  callNonterminalProcedure(&Parser::conditional_expression, self);

  return self;
}
std::shared_ptr<Node> Parser::logical_or_expression() 
{
  // left refactoring is any number of (logical_and_expression || logical_and_expression || ... || logical_and_expression)
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::LOGICAL_OR_EXPRESSION;

  callNonterminalProcedure(&Parser::logical_and_expression, self);
  while (HandleTerminal(TokenType::OR_OR, Type::LOGICAL_OR_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::logical_and_expression, self);
  }

  return self;
}
std::shared_ptr<Node> Parser::logical_and_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::LOGICAL_AND_EXPRESSION;

  callNonterminalProcedure(&Parser::inclusive_or_expression, self);
  while (HandleTerminal(TokenType::AND_AND, Type::LOGICAL_AND_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::inclusive_or_expression, self);
  }
  
  return self;
}
std::shared_ptr<Node> Parser::inclusive_or_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::INCLUSIVE_OR_EXPRESSION;

  callNonterminalProcedure(&Parser::exclusive_or_expression, self);
  while (HandleTerminal(TokenType::OR, Type::BITWISE_OR_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::exclusive_or_expression, self);
  }

  return self;
}

std::shared_ptr<Node> Parser::exclusive_or_expression()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::EXCLUSIVE_OR_EXPRESSION;
  
  callNonterminalProcedure(&Parser::and_expression, self);
  while (HandleTerminal(TokenType::XOR, Type::XOR_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::and_expression, self);
  }

  return self;
}
std::shared_ptr<Node> Parser::and_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::BITWISE_AND_EXPRESSION;

  callNonterminalProcedure(&Parser::equality_expression, self);
  while (HandleTerminal(TokenType::AND, Type::BITWISE_AND_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::equality_expression, self);
  }
  
  return self;
}
std::shared_ptr<Node> Parser::equality_expression() 
{
  // left-refactored is: (relational_expression [!= or ==] relational_expression [!= or ==] ... [!= or ==] relational_expression)
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::EQUALITY_EXPRESSION;

  callNonterminalProcedure(&Parser::relational_expression, self);
  while (HandleTerminal(TokenType::NOT_EQUAL, Type::NOT_EQUAL_OPERATOR, self) || HandleTerminal(TokenType::EQUAL_EQUAL, Type::EQUALITY_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::relational_expression, self);
  }

  return self;
}
std::shared_ptr<Node> Parser::relational_expression()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::RELATIONAL_EXPRESSION;

  callNonterminalProcedure(&Parser::shift_expression, self);
  while (HandleTerminal(TokenType::LESS_THAN, Type::LESS_THAN_OPERATOR, self)
        || HandleTerminal(TokenType::GREATER_THAN, Type::GREATER_THAN_OPERATOR, self)
        || HandleTerminal(TokenType::LESS_EQUAL, Type::LESS_THAN_EQUAL_OPERATOR, self)
        || HandleTerminal(TokenType::GREATER_EQUAL, Type::GREATER_THAN_EQUAL_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::shift_expression, self);
  }

  return self;
}
std::shared_ptr<Node> Parser::shift_expression() {}
std::shared_ptr<Node> Parser::additive_expression() {}
std::shared_ptr<Node> Parser::multiplicative_expression() {}
std::shared_ptr<Node> Parser::cast_expression() {}
std::shared_ptr<Node> Parser::unary_expression() {}
std::shared_ptr<Node> Parser::postfix_expression() {}
std::shared_ptr<Node> Parser::primary_expression() {
  /*
   * Primary expression is the most basic form of expression, consisting of an
   * identifier, string-literal, or numeric-constant. It can be surrounded by
   * any number of parentheses?
   */
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::PRIMARY_EXPRESSION;

  // primary_expression is NOT an error handling node, so we don't save
  // m_tokenIndex here prior to getNextToken
  Token nextToken = getNextToken();
  if (nextToken.type == TokenType::ID) {
    std::shared_ptr<Node> identifier_node = std::make_shared<Node>();
    identifier_node->type = Type::IDENTIFIER;
    identifier_node->accepted = true;
    // TODO: identifier_node.value or .data, etc should 'point' to an entry in a
    // symbol table describing the identifier For now, just the name
    identifier_node->data = nextToken.value;
    self->addChild((identifier_node));
  } else if (nextToken.type == TokenType::CONSTANT) {
    std::shared_ptr<Node> numeric_node = std::make_shared<Node>();
    numeric_node->type = Type::CONSTANT;
    numeric_node->accepted = true;
    numeric_node->data = nextToken.value;
    self->addChild((numeric_node));
  } else if (nextToken.type == TokenType::LITERAL) {
    std::shared_ptr<Node> string_node = std::make_shared<Node>();
    string_node->type = Type::STRING;
    string_node->accepted = true;
    // Should be in string literal table...
    string_node->data = nextToken.value;
    self->addChild((string_node));
  } else if (nextToken.type == TokenType::OPEN_PARENS) {
    std::shared_ptr<Node> left_parens = std::make_shared<Node>();
    std::shared_ptr<Node> expression_node = expression();
    std::shared_ptr<Node> right_parens = std::make_shared<Node>();

    left_parens->accepted = true;
    left_parens->type = Type::LEFT_PARENTHESIS;
    right_parens->type = Type::RIGHT_PARENTHESIS;

    if (!expression_node->accepted) {
      self->accepted = false;
    } else {
      nextToken = getNextToken();
      if (nextToken.type != TokenType::CLOSE_PARENS) {
        right_parens->accepted = false;
        right_parens->data = nextToken.value;
        self->addChild((right_parens));
      } else {
        self->addChild((right_parens));
      }
    }
    self->addChild((left_parens));
    self->addChild((expression_node));
  } else {
    std::shared_ptr<Node> error_node = std::make_shared<Node>();
    error_node->type = Type::ERROR;
    error_node->data = nextToken.value;
    error_node->accepted = false;
    self->addChild((error_node));
  }

  return self;
}
std::shared_ptr<Node> Parser::constant() {}

std::shared_ptr<Node> Parser::expression() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::EXPRESSION;

  while(callNonterminalProcedure(&Parser::assignment_expression, self)) {
    if (!HandleTerminal(TokenType::COMMA, Type::COMMA, self)) {
      break; 
    };
  }

  return self; 
}

std::shared_ptr<Node> Parser::assignment_expression() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ASSIGNMENT_EXPRESSION;

  int saved_input_index = m_tokenIndex;

  std::shared_ptr<Node> conditional_expression_node = conditional_expression();
  if (conditional_expression_node->accepted) {
    self->addChild((conditional_expression_node));
  } else {
    m_tokenIndex = saved_input_index;

    std::shared_ptr<Node> unary_expression_node = unary_expression();
    std::shared_ptr<Node> assignment_operator_node = assignment_operator();
    std::shared_ptr<Node> assignment_expression_node = assignment_expression();

    self->addChild((unary_expression_node));
    self->addChild((assignment_operator_node));
    self->addChild((assignment_expression_node));

    if (!unary_expression_node->accepted ||
        !assignment_operator_node->accepted ||
        !assignment_expression_node->accepted) {
      self->accepted = false;
    }
  }

  return self;
}

std::shared_ptr<Node> Parser::conditional_expression() {}

std::shared_ptr<Node> Parser::assignment_operator() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ASSIGNMENT_OPERATOR;

  Token tok = getNextToken();

  if (tok.type == TokenType::EQUAL) {
    // =
    std::shared_ptr<Node> assignment_node = std::make_shared<Node>();
    assignment_node->accepted = true;
    assignment_node->type = Type::UNARY_ASSIGNMENT;
    assignment_node->data = tok.value;
    self->addChild((assignment_node));
  } else if (tok.type == TokenType::STAR_EQUAL) {
    // *=
    std::shared_ptr<Node> assign_multiply_result_node =
        std::make_shared<Node>();
    assign_multiply_result_node->accepted = true;
    assign_multiply_result_node->type = Type::MULTIPLICATION_ASSIGNMENT;
    assign_multiply_result_node->data = tok.value;
    self->addChild((assign_multiply_result_node));
  } else if (tok.type == TokenType::SLASH_EQUAL) {
    // /=
    std::shared_ptr<Node> assign_divide_result_node = std::make_shared<Node>();
    assign_divide_result_node->accepted = true;
    assign_divide_result_node->type = Type::DIVISION_ASSIGNMENT;
    assign_divide_result_node->data = tok.value;
    self->addChild((assign_divide_result_node));
  } else if (tok.type == TokenType::PERCENT_EQUAL) {
    // %=
    std::shared_ptr<Node> assign_modulus_result_node = std::make_shared<Node>();
    assign_modulus_result_node->accepted = true;
    assign_modulus_result_node->type = Type::MODULUS_ASSIGNMENT;
    assign_modulus_result_node->data = tok.value;
    self->addChild((assign_modulus_result_node));
  } else if (tok.type == TokenType::PLUS_EQUAL) {
    // +=
    std::shared_ptr<Node> assign_addition_result_node =
        std::make_shared<Node>();
    assign_addition_result_node->accepted = true;
    assign_addition_result_node->type = Type::ADDITION_ASSIGNMENT;
    assign_addition_result_node->data = tok.value;
    self->addChild((assign_addition_result_node));
  } else if (tok.type == TokenType::MINUS_EQUAL) {
    // -=
    std::shared_ptr<Node> assign_subtraction_result_node =
        std::make_shared<Node>();
    assign_subtraction_result_node->accepted = true;
    assign_subtraction_result_node->type = Type::SUBTRACTION_ASSIGNMENT;
    assign_subtraction_result_node->data = tok.value;
    self->addChild((assign_subtraction_result_node));
  } else if (tok.type == TokenType::LESS_LESS_EQUAL) {
    // <<=
    std::shared_ptr<Node> assign_left_shift_result_node =
        std::make_shared<Node>();
    assign_left_shift_result_node->accepted = true;
    assign_left_shift_result_node->type = Type::LEFTSHIFT_ASSIGNMENT;
    assign_left_shift_result_node->data = tok.value;
    self->addChild((assign_left_shift_result_node));
  } else if (tok.type == TokenType::GREATER_GREATER_EQUAL) {
    // >>=
    std::shared_ptr<Node> assign_right_shift_result_node =
        std::make_shared<Node>();
    assign_right_shift_result_node->accepted = true;
    assign_right_shift_result_node->type = Type::RIGHTSHIFT_ASSIGNMENT;
    assign_right_shift_result_node->data = tok.value;
    self->addChild((assign_right_shift_result_node));
  } else if (tok.type == TokenType::AND_EQUAL) {
    // &=
    std::shared_ptr<Node> assign_bitwise_and_result_node =
        std::make_shared<Node>();
    assign_bitwise_and_result_node->accepted = true;
    assign_bitwise_and_result_node->type = Type::BITWISE_AND_ASSIGNMENT;
    assign_bitwise_and_result_node->data = tok.value;
    self->addChild((assign_bitwise_and_result_node));
  } else if (tok.type == TokenType::XOR_EQUAL) {
    // ^=
    std::shared_ptr<Node> assign_xor_result_node = std::make_shared<Node>();
    assign_xor_result_node->accepted = true;
    assign_xor_result_node->type = Type::XOR_ASSIGNMENT;
    assign_xor_result_node->data = tok.value;
    self->addChild((assign_xor_result_node));
  } else if (tok.type == TokenType::OR_EQUAL) {
    // |=
    std::shared_ptr<Node> assign_bitwise_or_result_node =
        std::make_shared<Node>();
    assign_bitwise_or_result_node->accepted = true;
    assign_bitwise_or_result_node->type = Type::BITWISE_OR_ASSIGNMENT;
    assign_bitwise_or_result_node->data = tok.value;
    self->addChild((assign_bitwise_or_result_node));
  } else {  // Error, token does not match expected terminal.
    std::shared_ptr<Node> error_node = std::make_shared<Node>();
    error_node->type = Type::ERROR;
    error_node->data = tok.value;
    error_node->accepted = false;
    self->addChild((error_node));
  }

  return self;
}

std::shared_ptr<Node> Parser::unary_operator() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::UNARY_OPERATOR;

  Token tok = getNextToken();

  if (tok.type == TokenType::AND) {
    std::shared_ptr<Node> unary_and_node = std::make_shared<Node>();
    unary_and_node->accepted = true;
    unary_and_node->type = Type::UNARY_AND;
    unary_and_node->data = tok.value;
    self->addChild((unary_and_node));
  } else if (tok.type == TokenType::STAR) {
    std::shared_ptr<Node> pointer_op_node = std::make_shared<Node>();
    pointer_op_node->accepted = true;
    pointer_op_node->type = Type::REFERENCE;
    pointer_op_node->data = tok.value;
    self->addChild((pointer_op_node));
  } else if (tok.type == TokenType::PLUS) {
    std::shared_ptr<Node> unary_positive_node = std::make_shared<Node>();
    unary_positive_node->accepted = true;
    unary_positive_node->type = Type::UNARY_POSITIVE;
    unary_positive_node->data = tok.value;
    self->addChild((unary_positive_node));
  } else if (tok.type == TokenType::MINUS) {
    std::shared_ptr<Node> unary_negative_node = std::make_shared<Node>();
    unary_negative_node->accepted = true;
    unary_negative_node->type = Type::UNARY_NEGATIVE;
    unary_negative_node->data = tok.value;
    self->addChild((unary_negative_node));
  } else if (tok.type == TokenType::TILDE) {
    std::shared_ptr<Node> bitwise_not_node = std::make_shared<Node>();
    bitwise_not_node->accepted = true;
    bitwise_not_node->type = Type::BITWISE_NOT;
    bitwise_not_node->data = tok.value;
    self->addChild((bitwise_not_node));
  } else if (tok.type == TokenType::NOT) {
    std::shared_ptr<Node> unary_not_node = std::make_shared<Node>();
    unary_not_node->accepted = true;
    unary_not_node->type = Type::UNARY_NOT;
    unary_not_node->data = tok.value;
    self->addChild((unary_not_node));
  } else {
    std::shared_ptr<Node> error_node = std::make_shared<Node>();
    error_node->type = Type::ERROR;
    error_node->data = tok.value;
    error_node->accepted = false;
    self->addChild((error_node));
  }

  return self;
}
std::shared_ptr<Node> Parser::type_name() {}
std::shared_ptr<Node> Parser::parameter_type_list() {}
std::shared_ptr<Node> Parser::parameter_list() {}
std::shared_ptr<Node> Parser::parameter_declaration() {}
std::shared_ptr<Node> Parser::abstract_declarator() {}
std::shared_ptr<Node> Parser::direct_abstract_declarator() {}
std::shared_ptr<Node> Parser::enum_specifier() {}
std::shared_ptr<Node> Parser::enumerator_list() {}
std::shared_ptr<Node> Parser::enumerator() {}
std::shared_ptr<Node> Parser::typedef_name() {}
std::shared_ptr<Node> Parser::declaration() {}
std::shared_ptr<Node> Parser::init_declarator() {
  std::shared_ptr<Node> self = std::make_shared<Node>();

  std::shared_ptr<Node> declarator_node = declarator();
  self->addChild(declarator_node);

  int saved_token_index = m_tokenIndex;
  Token nextToken = getNextToken();
  if (declarator_node->accepted && nextToken.type == TokenType::EQUAL) {
    std::shared_ptr<Node> assignment_op_node = std::make_shared<Node>();
    self->addChild(assignment_op_node);
    saved_token_index = m_tokenIndex;

    std::shared_ptr<Node> initializer_node = initializer();
    self->addChild(initializer_node);
  } else {
    m_tokenIndex = saved_token_index;
  }

  return self;
}
std::shared_ptr<Node> Parser::initializer() {
  std::shared_ptr<Node> self = std::make_shared<Node>();

  std::shared_ptr<Node> assignment_expression_node = assignment_expression();
  if (assignment_expression_node->accepted) {
    self->addChild(assignment_expression_node);
  } else {
    std::shared_ptr<Node> initializer_list_node = initializer_list();
    if (initializer_list_node->accepted) {
      self->addChild(initializer_list_node);
    } else {
      // TODO: implement
      // This function (won't have to be used much) calculates how far a node
      // has been able to parse It traverses the depth and breadth of a tree,
      // stopping in each direction when it finds a terminal mismatch.
      // Technically i guess it could be a member method of Node...
      if (computeTreeSize(assignment_expression_node) >
          computeTreeSize(initializer_list_node)) {
        self->addChild(assignment_expression_node);
      } else {
        self->addChild(initializer_list_node);
      }
    }
  }

  return self;
}
std::shared_ptr<Node> Parser::initializer_list() {}
std::shared_ptr<Node> Parser::compound_statement() 
{
  
}

std::shared_ptr<Node> Parser::statement() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  int saved_token_index = m_tokenIndex;

  if (callNonterminalProcedure(&Parser::labeled_statement, self)) {
    return self;
  }

  if (callNonterminalProcedure(&Parser::expression_statement, self)) {
    return self;
  }

  if (callNonterminalProcedure(&Parser::compound_statement, self)) {
    return self;
  }

  if (callNonterminalProcedure(&Parser::selection_statement, self)) {
    return self;
  }

  if (callNonterminalProcedure(&Parser::iteration_statement, self)) {
    return self;
  }

  if (callNonterminalProcedure(&Parser::jump_statement, self)) {
    return self;
  }
}

std::shared_ptr<Node> Parser::labeled_statement() {
  
}
std::shared_ptr<Node> Parser::expression_statement() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::EXPRESSION_STATEMENT;

  callNonterminalProcedure(&Parser::expression, self,
                           m_tokenIndex);

  return self;

}
std::shared_ptr<Node> Parser::selection_statement() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::SELECTION_STATEMENT;
  int start_index = m_tokenIndex;
  Token tok = getNextToken();

  if (tok.type == TokenType::IF && getNextToken().type == TokenType::OPEN_PARENS)
  {
    if (callNonterminalProcedure(&Parser::expression, self,
                                 m_tokenIndex))
    {
      if (getNextToken().type == TokenType::CLOSE_PARENS)
      {
        if (callNonterminalProcedure(&Parser::statement, self,
                                     m_tokenIndex))
        {
          if (getNextToken().type == TokenType::ELSE)
          {
            if (callNonterminalProcedure(&Parser::statement, self,
                                         m_tokenIndex))
            {
              return self;
            }
          }
          else
          {
            return self;
          }
        }
      }
    }
  }

  m_tokenIndex = start_index;
  if (tok.type == TokenType::SWITCH)
  {
    if (getNextToken().type == TokenType::OPEN_PARENS)
    {
      if (callNonterminalProcedure(&Parser::expression, self,
                                   m_tokenIndex))
      {
        if (getNextToken().type == TokenType::CLOSE_PARENS)
        {
          if (callNonterminalProcedure(&Parser::statement, self,
                                       m_tokenIndex))
          {
            return self;
          }
        }
      }
    }
  }
} 

std::shared_ptr<Node> Parser::iteration_statement() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ITERATION_STATEMENT;
  Token tok = getNextToken();
  int start_index = m_tokenIndex;

  if (tok.type == TokenType::WHILE && getNextToken().type == TokenType::OPEN_PARENS)
  {
    if (callNonterminalProcedure(&Parser::expression, self,
                                 m_tokenIndex))
    {
      if (getNextToken().type == TokenType::CLOSE_PARENS)
      {
        if (callNonterminalProcedure(&Parser::statement, self,
                                     m_tokenIndex))
        {
          return self;
        }
      }
    }
  }

  m_tokenIndex = start_index;
  if (tok.type == TokenType::DO)
  {
    if (callNonterminalProcedure(&Parser::statement, self,
                                 m_tokenIndex))
    {
      if (tok.type == TokenType::WHILE && getNextToken().type == TokenType::OPEN_PARENS)
      {
        if (callNonterminalProcedure(&Parser::expression, self,
                                     m_tokenIndex))
        {
          if (getNextToken().type == TokenType::CLOSE_PARENS)
          {
            if (callNonterminalProcedure(&Parser::statement, self,
                                         m_tokenIndex))
            {
              return self;
            }
          }
        }
      }
    }
  }

  m_tokenIndex = start_index;
  if (tok.type == TokenType::FOR && getNextToken().type == TokenType::OPEN_PARENS)
  {
    callNonterminalProcedure(&Parser::expression, self, m_tokenIndex);
    if (getNextToken().type == TokenType::SEMICOLON)
    {
      callNonterminalProcedure(&Parser::expression, self, m_tokenIndex);
      if (getNextToken().type == TokenType::SEMICOLON)
      {
        callNonterminalProcedure(&Parser::expression, self, m_tokenIndex);
        if (getNextToken().type == TokenType::CLOSE_PARENS)
        {
          if (callNonterminalProcedure(&Parser::statement, self,m_tokenIndex))
          {
            return self;
          }
        }
      }
    }
  }
}


// TODO: test this.  I did it weird. (iterative instead of recursive to wrap my
// head around left-refactoring)
std::shared_ptr<Node> Parser::jump_statement() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::JUMP_STATEMENT;

  Token tok = getNextToken();

  if (tok.type == TokenType::GOTO) {
    std::shared_ptr<Node> goto_node = std::make_shared<Node>();
    goto_node->accepted = true;
    goto_node->type = Type::GOTO;
    goto_node->data = tok.value;
    self->addChild((goto_node));

    tok = getNextToken();
    if (tok.type == TokenType::ID) {
      std::shared_ptr<Node> goto_label = std::make_shared<Node>();
      goto_label->accepted = true;
      goto_node->type = Type::GOTO;
      goto_node->data = tok.value;

      self->addChild((goto_label));
    } else {
      self->accepted = false;
    }
  } else if (tok.type == TokenType::CONTINUE) {
    std::shared_ptr<Node> continue_node = std::make_shared<Node>();
    continue_node->accepted = true;
    continue_node->type = Type::CONTINUE;
    continue_node->data = tok.value;
    self->addChild((continue_node));
  } else if (tok.type == TokenType::BREAK) {
    std::shared_ptr<Node> break_node = std::make_shared<Node>();
    break_node->accepted = true;
    break_node->type = Type::BREAK;
    break_node->data = tok.value;
    self->addChild((break_node));
  } else {
    std::shared_ptr<Node> error_node = std::make_shared<Node>();
    error_node->type = Type::ERROR;
    error_node->data = tok.value;
    error_node->accepted = false;
    self->addChild((error_node));

    return self;
  }

  // Next input should be semi-colon, for all possible productions
  tok = getNextToken();
  if (tok.type == TokenType::SEMICOLON) {
    std::shared_ptr<Node> semicolon_node = std::make_shared<Node>();
    semicolon_node->accepted = true;
    semicolon_node->type = Type::STATEMENT_END;
    semicolon_node->data = tok.value;
    self->addChild((semicolon_node));
  } else {
    self->accepted = false;
  }

  return self;
}

bool Parser::callNonterminalProcedure(std::shared_ptr<Node> (Parser::*fn)(),
                                      std::shared_ptr<Node> self,
                                      bool optional) {
  int tokenIndex = m_tokenIndex;
  std::shared_ptr<Node> node = (this->*fn)();
  if (optional == false) {
    self->addChild(node);
  }
  if (node->accepted) {
    if (optional == true) {
      self->addChild(node);
    }
    return true;
  } else {
    m_tokenIndex = tokenIndex;
    return false;
  }
}

std::shared_ptr<Node> Parser::keyword_if() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::IF;

  Token tok = getNextToken();
  if (tok.type == TokenType::IF) {
    self->accepted = true;
    self->data = tok.value;
  } else {
    self->accepted = false;
  }
  return self;
}

bool Parser::HandleTerminal(TokenType token_type, Type node_type, std::shared_ptr<Node> self, bool mandatory) {
  std::shared_ptr<Node> terminal_node = std::make_shared<Node>();

  int tokenIndex = m_tokenIndex;
  Token tok = getNextToken();

  if(tok.type == token_type) {
    self->type = node_type;
    self->accepted = true;
    self->addChild(terminal_node);
    return true;
  } else {
    if (mandatory) {
      self->type = Type::ERROR;
      self->data = getNextToken().value;
      self->accepted = false;
      self->addChild(terminal_node);
    } else {
      m_tokenIndex = tokenIndex;
      return false;
    }
  }
}

bool Parser::HandleUnexpectedTerminal(std::shared_ptr<Node> self)
{
  std::shared_ptr<Node> unexpected_terminal = std::make_shared<Node>();
  unexpected_terminal->type = Type::ERROR;

  unexpected_terminal->data = getNextToken().value;

  unexpected_terminal->accepted = false;
  self->addChild(unexpected_terminal);
}
