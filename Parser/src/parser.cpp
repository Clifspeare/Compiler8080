#include "parser.h"

#include <memory>
#include <iostream>

Parser::Parser() : m_preprocessor(), m_root(), m_errors(), m_tokens()
{
    m_root = std::make_unique<Node>();
    Token tok = m_preprocessor.getNextToken();
    while (tok.value != "end" && tok.value != "eof") {
        m_tokens.push_back(tok);
        tok = m_preprocessor.getNextToken();
    }
}

Token Parser::getNextToken()
{
    return m_tokens.at(m_tokenIndex++);
}

bool Parser::BuildTree() {
    m_root = translation_unit();
}

void Parser::reportError(const std::string& errorString)
{
    m_errors.push_back(errorString);
}

void Parser::showErrors()
{
    for (auto string : m_errors) {
        std::cout << string << std::endl;
    }
}

std::unique_ptr<Node> Parser::translation_unit()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::TRANSLATION_UNIT;

    std::unique_ptr<Node> external_declaration_node = external_declaration();

    while (external_declaration_node != nullptr) {
        self->children.push_back(std::move(external_declaration_node));
        external_declaration_node = external_declaration();
    }

    if (self->children.size() < 1 && m_errors.size() == 0) { // no external declarations formed but no errors reported
        reportError("Error: end-of-input reached - file is empty");
        showErrors();
        return nullptr;
    } else if (m_errors.size() == 0) { // external declarations formed and no errors
        return self;
    } else if (m_errors.size() > 0) { // errors found
        showErrors();
        return nullptr;
    }
}

std::unique_ptr<Node> Parser::external_declaration()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();

    std::unique_ptr<Node> function_definition_node = function_definition();
    std::unique_ptr<Node> declaration_node = declaration();

    // int count = 0;
    // while (true) {
    //     // CHECK FOR GLOBAL VARIABLES OR FUNCTIONS
    //     if (function_definition(child) || declaration(child)) {
    //         node->type = Type::EXTERNAL_DECLARATION;
    //         node->children.push_back(std::move(child));
    //         ++count;
    //     } else {
    //         if (count == 0)
    //             return false;
    //         else
    //             return true;
    //     }
    //     child = std::make_unique<Node>();
    // }
}

std::unique_ptr<Node> Parser::function_definition()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::FUNCTION_DEFINITION;

    // CHECK FOR RETURN VALUES - TODO: Handle special cases (int int, long long long, ect..)
    std::unique_ptr<Node> declaration_specifier_node;
    while (declaration_specifier(child));
    // CHECK FOR FUNCTION NAME
    if (!declarator(child))
        return false;
    // CHECK FOR PARAMETERS
    while (declaration(child));
    // CHECK FOR FUNCTION BODY
    if (!compound_statement(child))
        return false;

    node->children.push_back(std::move(child));
}
bool Parser::declaration_specifier(std::unique_ptr<Node>& node)
{
    std::unique_ptr<Node> child = std::make_unique<Node>();
    if (storage_class_specifier(child) || type_specifier(child) || type_qualifier(child)) {
        node->type = Type::DECLARATION_SPECIFIER;
        node->children.push_back(std::move(child));
    }
    return false;
}
bool Parser::storage_class_specifier(std::unique_ptr<Node>& node)
{
    if (CheckToken(std::vector<TokenType>({TokenType::AUTO, TokenType::REGISTER, TokenType::STATIC, TokenType::EXTERN, TokenType::TYPEDEF}))) {
        // child node of storage_class_specifier OR *value* of storage_class_specifier
    }
}
bool Parser::type_specifier(std::unique_ptr<Node>& node)
{
    
}
bool Parser::struct_or_union_specifer(std::unique_ptr<Node>& node) {}
bool Parser::struct_or_union(std::unique_ptr<Node>& node) {}
bool Parser::struct_declaration(std::unique_ptr<Node>& node) {}
bool Parser::specifier_qualifier(std::unique_ptr<Node>& node) {}
bool Parser::struct_declarator_list(std::unique_ptr<Node>& node) {}
bool Parser::struct_declarator(std::unique_ptr<Node>& node) {}
bool Parser::declarator(std::unique_ptr<Node>& node)
{
    std::unique_ptr<Node> child = std::make_unique<Node>();
    pointer(child); // zero or one
    if (direct_declarator(child)) {
        return false;
    }

    node->type = Type::DECLARATOR;
    node->children.push_back(std::move(child));
}
bool Parser::pointer(std::unique_ptr<Node>& node)
{
    
}
bool Parser::type_qualifier(std::unique_ptr<Node>& node) {}
bool Parser::direct_declarator(std::unique_ptr<Node>& node) {}
bool Parser::constant_expression(std::unique_ptr<Node>& node) {}
bool Parser::logical_or_expression(std::unique_ptr<Node>& node) {}
bool Parser::logical_and_expression(std::unique_ptr<Node>& node) {}
bool Parser::inclusive_or_expression(std::unique_ptr<Node>& node) {}
bool Parser::and_expression(std::unique_ptr<Node>& node) {}
bool Parser::equality_expression(std::unique_ptr<Node>& node) {}
bool Parser::relational_expression(std::unique_ptr<Node>& node) {}
bool Parser::shift_expression(std::unique_ptr<Node>& node) {}
bool Parser::additive_expression(std::unique_ptr<Node>& node) {}
bool Parser::multiplicative_expression(std::unique_ptr<Node>& node) {}
bool Parser::cast_expression(std::unique_ptr<Node>& node) {}
bool Parser::unary_expression(std::unique_ptr<Node>& node) {}
bool Parser::postfix_expression(std::unique_ptr<Node>& node) {}
std::unique_ptr<Node> Parser::primary_expression()
{
    /*
    * Primary expression is the most basic form of expression, consisting of an identifier, string-literal, or numeric-constant.
    * It can be surrounded by any number of parentheses?
    */
   std::unique_ptr<Node> self = std::make_unique<Node>();
   self->type = Type::PRIMARY_EXPRESSION;

   // primary_expression is NOT an error handling node, so we don't save m_tokenIndex here prior to getNextToken
   Token nextToken = getNextToken();
   if (nextToken.type == TokenType::ID) {
       std::unique_ptr<Node> identifier_node = std::make_unique<Node>();
       identifier_node->type = Type::IDENTIFIER;
       identifier_node->accepted = true;
       // TODO: identifier_node.value or .data, etc should 'point' to an entry in a symbol table describing the identifier
       // For now, just the name
       identifier_node->data = nextToken.value;
       self->children.push_back(std::move(identifier_node));
   } else if (nextToken.type == TokenType::CONSTANT) {
       std::unique_ptr<Node> numeric_node = std::make_unique<Node>();
       numeric_node->type = Type::CONSTANT;
       numeric_node->accepted = true;
       numeric_node->data = nextToken.value;
       self->children.push_back(std::move(numeric_node));
   } else if (nextToken.type == TokenType::LITERAL) {
       std::unique_ptr<Node> string_node = std::make_unique<Node>();
       string_node->type = Type::STRING;
       string_node->accepted = true;
       // Should be in string literal table...
       string_node->data = nextToken.value;
       self->children.push_back(std::move(string_node));
   } else if (nextToken.type == TokenType::OPEN_PARENS) {
       std::unique_ptr<Node> left_parens = std::make_unique<Node>();
       std::unique_ptr<Node> expression_node = expression();
       std::unique_ptr<Node> right_parens = std::make_unique<Node>();

       left_parens->accepted = true;
       self->children.push_back(std::move(left_parens));
       self->children.push_back(std::move(expression_node));
       if (!expression_node->accepted) {
           self->accepted = false;
       } else {
           nextToken = getNextToken();
           if (nextToken.type != TokenType::CLOSE_PARENS) {
               self->accepted = false;
               std::unique_ptr<Node> error_node = std::make_unique<Node>();
               error_node->type = Type::ERROR;
               error_node->data = nextToken.value;
               self->children.push_back(error_node);
            } else {
                self->children.push_back(std::move(right_parens));
                self->accepted = true;
            }
       }
   } else {
       std::unique_ptr<Node> error_node = std::make_unique<Node>();
       error_node->type = Type::ERROR;
       error_node->data = nextToken.value;
       self->children.push_back(error_node);
       self->accepted = false;
   }

   return self;
}
std::unique_ptr<Node> Parser::constant() {}
std::unique_ptr<Node> Parser::expression()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::EXPRESSION;

    std::unique_ptr<Node> assignment_expression_node = assignment_expression();
    if (assignment_expression_node->accepted) {
        self->accepted = true;
        self->children.push_back(std::move(assignment_expression_node));
    } else {
        // next rule, Expression ::= Expression , Assignment-Expression
        // This is left-recursive, and so I will leave it for later.
    }

}
std::unique_ptr<Node> Parser::assignment_expression() {}
bool Parser::assignment_operator(std::unique_ptr<Node>& node) {}
bool Parser::unary_operator(std::unique_ptr<Node>& node)
{
    CheckToken(std::vector<TokenType>({}));
}
bool Parser::type_name(std::unique_ptr<Node>& node) {}
bool Parser::parameter_type_list(std::unique_ptr<Node>& node) {}
bool Parser::parameter_list(std::unique_ptr<Node>& node) {}
bool Parser::parameter_declaration(std::unique_ptr<Node>& node) {}
bool Parser::abstract_declarator(std::unique_ptr<Node>& node) {}
bool Parser::direct_abstract_declarator(std::unique_ptr<Node>& node) {}
bool Parser::enum_specifer(std::unique_ptr<Node>& node) {}
bool Parser::enumerator_list(std::unique_ptr<Node>& node) {}
bool Parser::enumerator(std::unique_ptr<Node>& node) {}
bool Parser::typedef_name(std::unique_ptr<Node>& node) {}
std::unique_ptr<Node> Parser::declaration() {}
bool Parser::init_declarator(std::unique_ptr<Node>& node) {}
bool Parser::initializer(std::unique_ptr<Node>& node) {}
bool Parser::initializer_list(std::unique_ptr<Node>& node) {}
bool Parser::compound_statement(std::unique_ptr<Node>& node) {}
bool Parser::statement(std::unique_ptr<Node>& node) {}
bool Parser::labeled_statement(std::unique_ptr<Node>& node) {}
bool Parser::expression_statement(std::unique_ptr<Node>& node) {}
bool Parser::selection_statement(std::unique_ptr<Node>& node) {}
bool Parser::iteration_statement(std::unique_ptr<Node>& node) {}

bool Parser::jump_statement(std::unique_ptr<Node>& node)
{
    node->type = Type::JUMP_STATEMENT;
    if(CheckTokenPair(TokenType::GOTO,TokenType::ID)) {

        return true;
    }
}

bool Parser::CheckTokenPair(TokenType token1, TokenType token2) {
    // TODO: Do the buffer thing.
    return true;
}