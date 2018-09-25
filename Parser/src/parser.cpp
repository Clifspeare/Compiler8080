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


// To terminate, this function expects for 
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

  return ErrorInfo();
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
    bool compilation_error = !(callNonterminalProcedure(&Parser::external_declaration, self));

    if(compilation_error) {
      PrintErrorMessage(getErrorInfo(self));
    }
  }

  return self;
}

std::shared_ptr<Node> Parser::external_declaration() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  
  if(callNonterminalProcedure(&Parser::function_definition, self)) {
    return self;
  }

  callNonterminalProcedure(&Parser::declaration, self);

  return self;
}

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

std::shared_ptr<Node> Parser::declaration_specifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::DECLARATION_SPECIFIER;

  bool nonterminal_rejected = !(callNonterminalProcedure(&Parser::storage_class_specifier, self, true)
                            || callNonterminalProcedure(&Parser::type_specifier, self, true)
                            || callNonterminalProcedure(&Parser::type_qualifier, self, true));

  if (nonterminal_rejected) {
    // create and propogate error
  }

  return self;
}

std::shared_ptr<Node> Parser::storage_class_specifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::STORAGE_CLASS_SPECIFIER;

  bool terminal_rejected = !(HandleTerminal(TokenType::AUTO, Type::AUTO, self)
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

  if (HandleTerminal(TokenType::OPEN_BRACE, Type::OPEN_BRACE, self)) {
    while(callNonterminalProcedure(&Parser::struct_declaration, self));
    HandleTerminal(TokenType::CLOSE_BRACE, Type::CLOSE_BRACE, self, true);
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
    callNonterminalProcedure(&Parser::constant_expression, self);
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

std::shared_ptr<Node> Parser::pointer()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::POINTER;

  if(HandleTerminal(TokenType::STAR, Type::POINTER, self, true)) {
    while(callNonterminalProcedure(&Parser::type_qualifier, self, true));
    callNonterminalProcedure(&Parser::pointer, self, true);
  }

  return self;        
}

std::shared_ptr<Node> Parser::type_qualifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::TYPE_QUALIFIER;

  bool terminal_rejected = !(HandleTerminal(TokenType::CONST, Type::CONST_KEYWORD, self)
                         || HandleTerminal(TokenType::VOLATILE, Type::VOLATILE_KEYWORD, self));
  if (terminal_rejected) {
    HandleUnexpectedTerminal(self);
  }

  return self;
}

std::shared_ptr<Node> Parser::direct_declarator() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::DIRECT_DECLARATOR;

  bool identifier_branch = true;
  bool declarator_branch = true;
  bool const_expr_case = true;
  bool parameter_type_list_case = true;
  bool identifier_case = true;

  while(identifier_branch || declarator_branch) {
    // <identifier>
    identifier_branch = HandleTerminal(TokenType::ID, Type::IDENTIFIER, self);
    // ( <declarator> )
    declarator_branch = HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self)
                     && callNonterminalProcedure(&Parser::declarator, self, true)
                     && HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self);

    if(!identifier_branch && !declarator_branch) {
      break;
    }

    while(const_expr_case || parameter_type_list_case || identifier_case) {
      // { [ {constant-expression>}? ] }*
      const_expr_case = false;
      if(HandleTerminal(TokenType::OPEN_BRACKET, Type::OPEN_BRACKET, self)) {
        const_expr_case = true;
        callNonterminalProcedure(&Parser::constant_expression, self, true);
        if(!HandleTerminal(TokenType::CLOSE_BRACKET, Type::CLOSE_BRACKET, self)) {
          return self;
        }
      }

      // { ( <parameter-type-list> ) }*
      parameter_type_list_case = false;
      if(HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self)) {
        const_expr_case = true;
        callNonterminalProcedure(&Parser::parameter_type_list, self, true);
        if(!HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self)) {
          return self;
        }
      }

      // { ( {<identifer>}* ) }*
      identifier_case = false;
      if(HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self)) {
        const_expr_case = true;
        HandleTerminal(TokenType::ID, Type::IDENTIFIER, self);
        while (HandleTerminal(TokenType::COMMA, Type::COMMA, self)){
          HandleTerminal(TokenType::ID, Type::IDENTIFIER, self, true);          
        }
        if(!HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self)) {
          return self;
        }
      }      
    }
  }

  return self;
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

std::shared_ptr<Node> Parser::shift_expression()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::SHIFT_EXPRESSION;

  callNonterminalProcedure(&Parser::additive_expression, self);
  while (HandleTerminal(TokenType::LESS_LESS, Type::LEFTSHIFT_OPERATOR, self) || HandleTerminal(TokenType::GREATER_GREATER, Type::RIGHTSHIFT_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::additive_expression, self);
  }
  
  return self;
}
std::shared_ptr<Node> Parser::additive_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ADDITIVE_EXPRESSION;

  callNonterminalProcedure(&Parser::multiplicative_expression, self);
  while (HandleTerminal(TokenType::PLUS, Type::ADDITION_OPERATOR, self) || HandleTerminal(TokenType::MINUS, Type::SUBTRACTION_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::multiplicative_expression, self);
  }
  
  return self;
}

std::shared_ptr<Node> Parser::multiplicative_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::MULTIPLICATIVE_EXPRESSION;

  callNonterminalProcedure(&Parser::cast_expression, self);
  while (HandleTerminal(TokenType::STAR, Type::MULTIPLICATION_OPERATOR, self) 
        || HandleTerminal(TokenType::SLASH, Type::DIVISION_OPERATOR, self)
        || HandleTerminal(TokenType::PERCENT, Type::MODULUS_OPERATOR, self)) {
    callNonterminalProcedure(&Parser::cast_expression, self);
  }

  return self;
}

std::shared_ptr<Node> Parser::cast_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::CAST_EXPRESSION;

  callNonterminalProcedure(&Parser::unary_expression, self, true);

  if(HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self)) {
    callNonterminalProcedure(&Parser::type_name, self);
    if(HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true)) {
      return self;      
    }
    callNonterminalProcedure(&Parser::cast_expression, self);
  }

  return self;
}

std::shared_ptr<Node> Parser::unary_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::UNARY_EXPRESSION;

  callNonterminalProcedure(&Parser::postfix_expression, self, true);

  if(HandleTerminal(TokenType::PLUS_PLUS, Type::INCREMENT, self)) {
    if(callNonterminalProcedure(&Parser::unary_expression, self)) {
      return self;
    }
  }

  if(HandleTerminal(TokenType::MINUS_MINUS, Type::DECREMENT, self)) {
    if(callNonterminalProcedure(&Parser::unary_expression, self)){
      return self;
    }
  }

  if(callNonterminalProcedure(&Parser::unary_operator, self, true)) {
    if(callNonterminalProcedure(&Parser::cast_expression, self)){
      return self;            
    }    
  }

  if(HandleTerminal(TokenType::SIZEOF, Type::SIZEOF, self)) {
    if(callNonterminalProcedure(&Parser::unary_expression, self)) {
      return self;      
    }
  }

  if(HandleTerminal(TokenType::SIZEOF, Type::SIZEOF, self)) {
    if(callNonterminalProcedure(&Parser::type_name, self)) {
      return self;
    }
  }            

  return self;
}

std::shared_ptr<Node> Parser::postfix_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::POSTFIX_EXPRESSION;

  callNonterminalProcedure(&Parser::primary_expression, self);
  
  bool expr_case = true;
  bool assignment_expr_case = true;
  bool dot_identifier_case = true;
  bool ptr_identifier_case = true;
  bool postfix_increment_case = true;
  bool postfix_decrement_case = true;

  while (expr_case || assignment_expr_case || dot_identifier_case || ptr_identifier_case || postfix_decrement_case || postfix_decrement_case) {
    //
    if ((expr_case = HandleTerminal(TokenType::OPEN_BRACKET, Type::OPEN_BRACKET, self))) {
     callNonterminalProcedure(&Parser::expression, self);
      if(!HandleTerminal(TokenType::CLOSE_BRACKET, Type::CLOSE_BRACKET, self, true)) {
        return self;
      }
    }
    if (assignment_expr_case = HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self)) {
      while (callNonterminalProcedure(&Parser::assignment_expression, self, true));
      if (!HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true)) {
        return self;
      }
    }

    if(dot_identifier_case = HandleTerminal(TokenType::DOT, Type::ACCESS, self)){
      HandleTerminal(TokenType::ID, Type::IDENTIFIER, self, true);      
    }
    if(ptr_identifier_case = HandleTerminal(TokenType::SELECTION, Type::POINTER_MEMBER, self)){
      HandleTerminal(TokenType::ID, Type::IDENTIFIER, self, true);
    }
    postfix_increment_case = HandleTerminal(TokenType::PLUS_PLUS, Type::INCREMENT, self);
    postfix_decrement_case = HandleTerminal(TokenType::MINUS_MINUS, Type::DECREMENT, self);
  }

  return self;
}

std::shared_ptr<Node> Parser::primary_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::PRIMARY_EXPRESSION;

  HandleTerminal(TokenType::ID, Type::IDENTIFIER, self);
  HandleTerminal(TokenType::CONSTANT, Type::CONSTANT, self);
  HandleTerminal(TokenType::LITERAL, Type::STRING, self);
  if(HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self)) {
    callNonterminalProcedure(&Parser::expression, self);
    if(HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true)) {
      return self;      
    }
  }

  return self;
}

std::shared_ptr<Node> Parser::constant() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::CONSTANT;

  // TODO: TokenType::CONSTANT should be TokenType::INTEGER_CONSTANT
  HandleTerminal(TokenType::CONSTANT, Type::INTEGER_CONSTANT, self);
  HandleTerminal(TokenType::CHARACTER_CONSTANT, Type::CHARACTER_CONSTANT, self);
  HandleTerminal(TokenType::FLOATING_CONSTANT, Type::FLOATING_CONSTANT, self);

  return self;

}

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

  callNonterminalProcedure(&Parser::conditional_expression, self, true);

  if (callNonterminalProcedure(&Parser::unary_expression, self, true)) {
    callNonterminalProcedure(&Parser::assignment_operator, self);
    callNonterminalProcedure(&Parser::assignment_expression, self);
  }

  // TODO for error handling, disambiguate between branches.

  return self;
}

std::shared_ptr<Node> Parser::conditional_expression() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::CONDITIONAL_EXPRESSION;

  callNonterminalProcedure(&Parser::logical_or_expression, self);

  if(HandleTerminal(TokenType::QUESTION_MARK, Type::QUESTION_MARK, self)){
    callNonterminalProcedure(&Parser::expression, self);
    HandleTerminal(TokenType::COLON, Type::COLON, self);
    callNonterminalProcedure(&Parser::conditional_expression, self);
    }

  return self;
}

std::shared_ptr<Node> Parser::assignment_operator() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ASSIGNMENT_OPERATOR;

  bool terminal_rejected = !(HandleTerminal(TokenType::EQUAL, Type::ASSIGNMENT_OPERATOR, self)
                        ||   HandleTerminal(TokenType::STAR_EQUAL, Type::MULTIPLICATION_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::SLASH_EQUAL, Type::DIVISION_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::PERCENT_EQUAL, Type::MODULUS_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::PLUS_EQUAL, Type::ADDITION_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::MINUS_EQUAL, Type::SUBTRACTION_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::LESS_LESS_EQUAL, Type::LEFTSHIFT_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::GREATER_GREATER_EQUAL, Type::RIGHTSHIFT_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::AND_EQUAL, Type::BITWISE_AND_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::XOR_EQUAL, Type::XOR_ASSIGNMENT, self)
                        ||   HandleTerminal(TokenType::OR_EQUAL, Type::BITWISE_OR_ASSIGNMENT, self));

  if (terminal_rejected) {
    HandleUnexpectedTerminal(self);
  }

  return self;
}

std::shared_ptr<Node> Parser::unary_operator() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::UNARY_OPERATOR;

  bool tokens_rejected = !(HandleTerminal(TokenType::AND, Type::UNARY_AND, self)
                        || HandleTerminal(TokenType::STAR, Type::REFERENCE, self)
                        || HandleTerminal(TokenType::PLUS, Type::UNARY_POSITIVE, self)
                        || HandleTerminal(TokenType::MINUS, Type::UNARY_NEGATIVE, self)
                        || HandleTerminal(TokenType::TILDE, Type::BITWISE_NOT, self)
                        || HandleTerminal(TokenType::NOT, Type::UNARY_NOT, self));

  if (tokens_rejected) {
    HandleUnexpectedTerminal(self);
  }

  return self;
}

std::shared_ptr<Node> Parser::type_name() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::TYPE_NAME;

  if(callNonterminalProcedure(&Parser::specifier_qualifier, self)) {
    while(callNonterminalProcedure(&Parser::specifier_qualifier, self, true));
    callNonterminalProcedure(&Parser::abstract_declarator, self, true);
  }

  return self;

}

std::shared_ptr<Node> Parser::parameter_type_list() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::PARAMETER_TYPE_LIST;

  if(callNonterminalProcedure(&Parser::parameter_list, self)) {
    while(HandleTerminal(TokenType::COMMA, Type::COMMA, self)){
      callNonterminalProcedure(&Parser::parameter_list, self);
    }
  }

  return self;
}

std::shared_ptr<Node> Parser::parameter_list() // parameter_declaration , parameter_declaration ...
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::PARAMETER_LIST;

  callNonterminalProcedure(&Parser::parameter_declaration, self);

  while (HandleTerminal(TokenType::COMMA, Type::COMMA, self)) {
    callNonterminalProcedure(&Parser::parameter_declaration, self);
  }
  
  return self;
}

std::shared_ptr<Node> Parser::parameter_declaration() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::PARAMETER_DECLARATION;

  callNonterminalProcedure(&Parser::parameter_declaration, self);

  if ( !(callNonterminalProcedure(&Parser::abstract_declarator, self, true)) ) {
    callNonterminalProcedure(&Parser::declarator, self, true);
  }

  return self;
}

std::shared_ptr<Node> Parser::abstract_declarator() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ABSTRACT_DECLARATOR;

  if (! (callNonterminalProcedure(&Parser::pointer, self, true))) {
    callNonterminalProcedure(&Parser::direct_abstract_declarator, self, true);

    // do a premature return to short-circuit unnecessary processing of alternate production-rule.
    return self;
  }

  // for now, making this non-optional but in the future we should determine the "closer" of the two productions
  callNonterminalProcedure(&Parser::direct_abstract_declarator, self);

  return self;
}

std::shared_ptr<Node> Parser::direct_abstract_declarator()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ABSTRACT_DECLARATOR;

  HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self, true);
  callNonterminalProcedure(&Parser::abstract_declarator, self);
  HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true);

  bool param_type_list_case = true, const_expr_case = true;

  while (param_type_list_case || const_expr_case) {
    param_type_list_case = false;
    const_expr_case = false;
    
    if(HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self)){
      callNonterminalProcedure(&Parser::constant_expression, self, true);
      param_type_list_case = HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true);
    }
    else if(HandleTerminal(TokenType::OPEN_BRACKET, Type::OPEN_BRACKET, self)){
      callNonterminalProcedure(&Parser::constant_expression, self, true);
      const_expr_case = HandleTerminal(TokenType::CLOSE_BRACKET, Type::CLOSE_BRACKET, self, true);
    }
  }

  return self;
}

std::shared_ptr<Node> Parser::enum_specifier() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ENUM_SPECIFIER;

  HandleTerminal(TokenType::ENUM, Type::ENUM, self, true);

  HandleTerminal(TokenType::ID, Type::IDENTIFIER, self);

  if (HandleTerminal(TokenType::OPEN_BRACE, Type::OPEN_BRACE, self)) {
    callNonterminalProcedure(&Parser::enumerator_list, self, true);
    HandleTerminal(TokenType::CLOSE_BRACE, Type::CLOSE_BRACE, self);
  }

  return self;
}

std::shared_ptr<Node> Parser::enumerator_list() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ENUMERATOR_LIST;

  callNonterminalProcedure(&Parser::enumerator, self);
  while (HandleTerminal(TokenType::COMMA, Type::COMMA, self)) {
    callNonterminalProcedure(&Parser::enumerator, self);
  }

  return self;
}

std::shared_ptr<Node> Parser::enumerator() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ENUMERATOR;

  HandleTerminal(TokenType::ID, Type::IDENTIFIER, self, true);
  while (HandleTerminal(TokenType::EQUAL, Type::ASSIGNMENT_OPERATOR, self)) {
    HandleTerminal(TokenType::ID, Type::IDENTIFIER, self, true);
  }

  return self;
}

std::shared_ptr<Node> Parser::typedef_name() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::TYPEDEF_NAME;

  HandleTerminal(TokenType::ID, Type::IDENTIFIER, self, true);

  return self;
}

std::shared_ptr<Node> Parser::declaration()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::DECLARATION;

  while (callNonterminalProcedure(&Parser::declaration_specifier, self)) {
    ; // keep running declaration_specifier
  }

  while (callNonterminalProcedure(&Parser::init_declarator, self, true)) {
    ; // processed in conditional
  }

  return self;
}

std::shared_ptr<Node> Parser::init_declarator() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::INIT_DECLARATOR;

  if(callNonterminalProcedure(&Parser::declarator, self)) {
    if (HandleTerminal(TokenType::EQUAL, Type::ASSIGNMENT_OPERATOR, self)) {
      callNonterminalProcedure(&Parser::initializer, self);      
    }
  }
  
  return self;
}

std::shared_ptr<Node> Parser::initializer() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::INITIALIZER;

  if (callNonterminalProcedure(&Parser::assignment_expression, self, true)) {
    return self;
  }

  if(HandleTerminal(TokenType::OPEN_BRACE, Type::OPEN_BRACE, self)) {
    callNonterminalProcedure(&Parser::initializer_list, self, true);
    HandleTerminal(TokenType::COMMA, Type::COMMA, self);
    HandleTerminal(TokenType::CLOSE_BRACE, Type::CLOSE_BRACE, self, true);
  }

  return self;
}

std::shared_ptr<Node> Parser::initializer_list() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::INITIALIZER_LIST;

  callNonterminalProcedure(&Parser::initializer, self);
  while (HandleTerminal(TokenType::COMMA, Type::COMMA, self)) {
    callNonterminalProcedure(&Parser::initializer, self);
  }

  return self;
}

std::shared_ptr<Node> Parser::compound_statement()
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::COMPOUND_STATEMENT;

  while (callNonterminalProcedure(&Parser::declaration, self, true)) {
    ; // Work in condition
  }

  while (callNonterminalProcedure(&Parser::declaration, self, true)) {
    ; // Work in condition
  }

  return self;
}

std::shared_ptr<Node> Parser::statement() {
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::INITIALIZER_LIST;

  callNonterminalProcedure(&Parser::labeled_statement, self, true);
  callNonterminalProcedure(&Parser::expression_statement, self, true);
  callNonterminalProcedure(&Parser::compound_statement, self, true);
  callNonterminalProcedure(&Parser::selection_statement, self, true);
  callNonterminalProcedure(&Parser::iteration_statement, self, true);
  callNonterminalProcedure(&Parser::jump_statement, self, true);

  // Add error info.
  HandleUnexpectedTerminal(self);

  return self;
}

std::shared_ptr<Node> Parser::labeled_statement() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::LABELED_STATEMENT;

  if(HandleTerminal(TokenType::ID, Type::IDENTIFIER, self)){
    HandleTerminal(TokenType::COLON, Type::COLON, self, true);
    callNonterminalProcedure(&Parser::statement, self);
    return self;    
  }

  if(HandleTerminal(TokenType::CASE, Type::CASE, self)) {
    callNonterminalProcedure(&Parser::expression, self);
    HandleTerminal(TokenType::COLON, Type::COLON, self, true);
    callNonterminalProcedure(&Parser::statement, self);
    return self;    
  }  

  if (HandleTerminal(TokenType::DEFAULT, Type::DEFAULT, self)) {
    HandleTerminal(TokenType::COLON, Type::COLON, self, true);
    callNonterminalProcedure(&Parser::statement, self);
    return self;
  }
  
  return HandleUnexpectedTerminal(self);
}

std::shared_ptr<Node> Parser::expression_statement() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::EXPRESSION_STATEMENT;

  callNonterminalProcedure(&Parser::expression, self, true);
  HandleTerminal(TokenType::SEMICOLON,Type::SEMICOLON, self, true);

  return self;
}

std::shared_ptr<Node> Parser::selection_statement() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::SELECTION_STATEMENT;

  if (HandleTerminal(TokenType::IF, Type::IF, self)) {
    HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self, true);
    callNonterminalProcedure(&Parser::expression, self);
    HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true);
    callNonterminalProcedure(&Parser::statement, self);

    if (HandleTerminal(TokenType::ELSE, Type::ELSE, self)) {
      callNonterminalProcedure(&Parser::statement, self);
    }
  }
  if (HandleTerminal(TokenType::SWITCH, Type::SWITCH, self)) {
    HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self, true);      
    callNonterminalProcedure(&Parser::expression, self);
    HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true);
    callNonterminalProcedure(&Parser::statement, self);
  }

  return self;
} 

std::shared_ptr<Node> Parser::iteration_statement() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::ITERATION_STATEMENT;

  if(HandleTerminal(TokenType::WHILE, Type::WHILE, self)){
    HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self, true);
    callNonterminalProcedure(&Parser::expression, self);
    HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true);
    callNonterminalProcedure(&Parser::statement, self);
    return self;           
  }
  
  if(HandleTerminal(TokenType::DO, Type::DO, self)){
    callNonterminalProcedure(&Parser::statement, self);
    HandleTerminal(TokenType::WHILE, Type::WHILE, self, true);        
    HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self, true);
    callNonterminalProcedure(&Parser::expression, self);
    HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true);
    HandleTerminal(TokenType::SEMICOLON, Type::SEMICOLON, self, true);    
    return self;
  }

  if(HandleTerminal(TokenType::FOR, Type::FOR, self)){
    HandleTerminal(TokenType::OPEN_PARENS, Type::LEFT_PARENTHESIS, self, true);

    for(int i = 0; i < 2; ++i) {
      callNonterminalProcedure(&Parser::expression, self, true);
      HandleTerminal(TokenType::SEMICOLON, Type::SEMICOLON, self, true);
    }
    callNonterminalProcedure(&Parser::expression, self, true);

    HandleTerminal(TokenType::CLOSE_PARENS, Type::RIGHT_PARENTHESIS, self, true);
    callNonterminalProcedure(&Parser::statement, self);
    
    return self;
  }

  return HandleUnexpectedTerminal(self);
}

std::shared_ptr<Node> Parser::jump_statement() 
{
  std::shared_ptr<Node> self = std::make_shared<Node>();
  self->type = Type::JUMP_STATEMENT;

  if(HandleTerminal(TokenType::GOTO, Type::GOTO, self)){
    HandleTerminal(TokenType::ID, Type::IDENTIFIER, self, true);
    HandleTerminal(TokenType::SEMICOLON, Type::SEMICOLON, self, true);
    return self;
  }

  if(HandleTerminal(TokenType::CONTINUE, Type::CONTINUE, self)){
    HandleTerminal(TokenType::SEMICOLON, Type::SEMICOLON, self, true);
    return self;
  }

  if(HandleTerminal(TokenType::BREAK, Type::BREAK, self)){
    HandleTerminal(TokenType::SEMICOLON, Type::SEMICOLON, self, true);
    return self;
  }

  if(HandleTerminal(TokenType::RETURN, Type::RETURN, self)){
    callNonterminalProcedure(&Parser::expression, self, true);
    HandleTerminal(TokenType::SEMICOLON, Type::SEMICOLON, self, true);
    return self;
  }

  return HandleUnexpectedTerminal(self);
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
    return false;
  }
}

std::shared_ptr<Node> Parser::HandleUnexpectedTerminal(std::shared_ptr<Node> self)
{
  std::shared_ptr<Node> unexpected_terminal = std::make_shared<Node>();
  unexpected_terminal->type = Type::ERROR;

  unexpected_terminal->data = getNextToken().value;

  unexpected_terminal->accepted = false;
  self->addChild(unexpected_terminal);

  return self;
}
