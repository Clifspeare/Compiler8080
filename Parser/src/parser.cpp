#include "parser.h"
#include <memory>

Parser::Parser()
{
    root = new Node();
}

bool Parser::BuildTree() {
    translation_unit(root);
}

bool translation_unit(std::unique_ptr<Node> node)
{
    std::unique_ptr<Node> child = std::make_unique<Node>();
    if (external_declaration(child)) {
        node->type = Type::TRANSLATION_UNIT;
        node->children.push_back(child);
        return true;
    }
    return false;
}

bool external_declaration(std::unique_ptr<Node> node)
{
    std::unique_ptr<Node> child = std::make_unique<Node>();
    int count = 0;
    while (true) {
        if (function_definition(child) || declaration(child)) {
            node->type = Type::EXTERNAL_DECLARATION;
            node->children.push_back(child);
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

bool function_definition(std::unique_ptr<Node> node);
bool declaration_specifier(std::unique_ptr<Node> node);
bool storage_class_specifier(std::unique_ptr<Node> node);
bool type_specifier(std::unique_ptr<Node> node);
bool struct_or_union_specifer(std::unique_ptr<Node> node);
bool struct_or_union(std::unique_ptr<Node> node);
bool struct_declaration(std::unique_ptr<Node> node);
bool specifier_qualifier(std::unique_ptr<Node> node);
bool struct_declarator_list(std::unique_ptr<Node> node);
bool struct_declarator(std::unique_ptr<Node> node);
bool declarator(std::unique_ptr<Node> node);
bool pointer(std::unique_ptr<Node> node);
bool type_qualifier(std::unique_ptr<Node> node);
bool direct_declarator(std::unique_ptr<Node> node);
bool constant_expression(std::unique_ptr<Node> node);
bool logical_or_expression(std::unique_ptr<Node> node);
bool logical_and_expression(std::unique_ptr<Node> node);
bool inclusive_or_expression(std::unique_ptr<Node> node);
bool and_expression(std::unique_ptr<Node> node);
bool equality_expression(std::unique_ptr<Node> node);
bool relational_expression(std::unique_ptr<Node> node);
bool shift_expression(std::unique_ptr<Node> node);
bool additive_expression(std::unique_ptr<Node> node);
bool multiplicative_expression(std::unique_ptr<Node> node);
bool cast_expression(std::unique_ptr<Node> node);
bool unary_expression(std::unique_ptr<Node> node);
bool postfix_expression(std::unique_ptr<Node> node);
bool primary_expression(std::unique_ptr<Node> node);
bool constant(std::unique_ptr<Node> node);
bool expression(std::unique_ptr<Node> node);
bool assignment_expression(std::unique_ptr<Node> node);
bool assignment_operator(std::unique_ptr<Node> node);
bool unary_operator(std::unique_ptr<Node> node)
{
    CheckToken(std::vector<TokenType>({}));
}
bool type_name(std::unique_ptr<Node> node);
bool parameter_type_list(std::unique_ptr<Node> node);
bool parameter_list(std::unique_ptr<Node> node);
bool parameter_declaration(std::unique_ptr<Node> node);
bool abstract_declarator(std::unique_ptr<Node> node);
bool direct_abstract_declarator(std::unique_ptr<Node> node);
bool enum_specifer(std::unique_ptr<Node> node);
bool enumerator_list(std::unique_ptr<Node> node);
bool enumerator(std::unique_ptr<Node> node);
bool typedef_name(std::unique_ptr<Node> node);
bool declaration(std::unique_ptr<Node> node);
bool init_declarator(std::unique_ptr<Node> node);
bool initializer(std::unique_ptr<Node> node);
bool initializer_list(std::unique_ptr<Node> node);
bool compound_statement(std::unique_ptr<Node> node);
bool statement(std::unique_ptr<Node> node);
bool labeled_statement(std::unique_ptr<Node> node);
bool expression_statement(std::unique_ptr<Node> node);
bool selection_statement(std::unique_ptr<Node> node);
bool iteration_statement(std::unique_ptr<Node> node);

bool jump_statement(std::unique_ptr<Node> node)
{
    node->type = Type::JUMP_STATEMENT;
    if(CheckTokenPair(TokenType::GOTO,TokenType::ID)) {
        
        return true;
    }
}

bool CheckTokenPair(TokenType token1, TokenType token2) {
    // TODO: Do the buffer thing.
}