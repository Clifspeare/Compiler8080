#include "parser.h"
#include <memory>

Parser::Parser() : m_root()
{
    m_root = std::make_unique<Node>();
}

bool Parser::BuildTree() {
    translation_unit(m_root);
}

bool Parser::translation_unit(std::unique_ptr<Node>& node)
{
    std::unique_ptr<Node> child = std::make_unique<Node>();
    if (external_declaration(child)) {
        node->type = Type::TRANSLATION_UNIT;
        node->children.push_back(std::move(child));
        return true;
    }
    return false;
}

bool Parser::external_declaration(std::unique_ptr<Node>& node)
{
    std::unique_ptr<Node> child = std::make_unique<Node>();
    int count = 0;
    while (true) {
        if (function_definition(child) || declaration(child)) {
            node->type = Type::EXTERNAL_DECLARATION;
            node->children.push_back(std::move(child));
            ++count;
        } else {
            if (count == 0)
                return false;
            else
                return true;
        }
        child = std::make_unique<Node>();
    }
}

bool Parser::function_definition(std::unique_ptr<Node>& node) {}
bool Parser::declaration_specifier(std::unique_ptr<Node>& node) {}
bool Parser::storage_class_specifier(std::unique_ptr<Node>& node) {}
bool Parser::type_specifier(std::unique_ptr<Node>& node) {}
bool Parser::struct_or_union_specifer(std::unique_ptr<Node>& node) {}
bool Parser::struct_or_union(std::unique_ptr<Node>& node) {}
bool Parser::struct_declaration(std::unique_ptr<Node>& node) {}
bool Parser::specifier_qualifier(std::unique_ptr<Node>& node) {}
bool Parser::struct_declarator_list(std::unique_ptr<Node>& node) {}
bool Parser::struct_declarator(std::unique_ptr<Node>& node) {}
bool Parser::declarator(std::unique_ptr<Node>& node) {}
bool Parser::pointer(std::unique_ptr<Node>& node) {}
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
bool Parser::declaration(std::unique_ptr<Node>& node) {}
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