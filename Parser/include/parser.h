#ifndef PARSER_H
#define PARSER_H

#include "Preprocessor.h"
#include "token_types.h"

enum class Type
{
    TRANSLATION_UNIT,
    EXTERNAL_DECLARATION,
    FUNCTION_DEFINITION,
    DECLARATION,
    DECLARATION_SPECIFIER,
    STORAGE_CLASS_SPECIFIER,
    TYPE_SPECIFIER
};

template <class T>
class SymbolEntry {
public:
    SymbolEntry(T new_symbol){
        symbol_m = new_symbol;
    }
    
    std::string identifier_name_m;
    T symbol_m;
    SymbolEntry* next_m;
};

class Node {
public:
    explicit Node(Node *parent) {this->parent = parent;}
    Node() {};

    std::string data;
    Type type;
    Node* parent;
    std::vector<std::unique_ptr<Node>> children;
};

class Parser {
public:
    Parser();
    bool BuildTree();

private:
    Preprocessor m_preprocessor;
    Node* root;

    bool translation_unit(std::unique_ptr<Node> node);
    bool external_declaration(std::unique_ptr<Node> node);
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
    bool unary_operator(std::unique_ptr<Node> node);
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
    bool jump_statement(std::unique_ptr<Node> node);

};

#endif PARSER_H
