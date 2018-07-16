#include "parser.h"

#include <memory>
#include <iostream>

Parser::Parser() : m_root()
{
    m_root = std::make_unique<Node>();
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

// bool Parser::translation_unit(std::unique_ptr<Node>& node)
// {
//     std::unique_ptr<Node> child = std::make_unique<Node>();

//     // PROCCESS FILE
//     if (external_declaration(child)) {
//         node->type = Type::TRANSLATION_UNIT;
//         node->children.push_back(std::move(child));
//         return true;
//     }
//     return false;
// }

// std::unique_ptr<Node> Parser::translation_unit(std::unique_ptr<Node> parent)
// {
//     std::unique_ptr<Node> self = std::make_unique<Node>();
//     self->type = Type::TRANSLATION_UNIT;

//     if (external_declaration_node != NULL) {
//         self->parent = parent;
//         parent->children.push_back(std::move(self));
//     } else {
//         // create error message with parent.children[0]->type and token recieved
//     }
// }

std::unique_ptr<Node> Parser::translation_unit()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::TRANSLATION_UNIT;

    std::unique_ptr<Node> external_declaration_node = external_declaration();

    /* There are two cases where external_declaration will return a nullptr:
    * 1. When the end of input is reached, and when there is an error.
    * 2. When an external_declaration can not be formed from the input (a parse error/malformed input)
    * To disambiguate between the two
    */

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
    std::unique_ptr<Node> declaration_specifier_node
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
bool Parser::primary_expression(std::unique_ptr<Node>& node) {}
bool Parser::constant(std::unique_ptr<Node>& node) {}
bool Parser::expression(std::unique_ptr<Node>& node) {}
bool Parser::assignment_expression(std::unique_ptr<Node>& node) {}
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