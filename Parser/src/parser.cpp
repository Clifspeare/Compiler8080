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

bool Parser::HasTokens()
{
    if(m_tokenIndex == m_tokens.size()){
        return false;
    }

    return true;    
}

Token Parser::getNextToken()
{
    Token t = m_tokens.at(m_tokenIndex++);
    return t;
}

struct ErrorInfo {
    Type expected_type;
    std::string actual_value;
};

ErrorInfo getErrorInfo(std::unique_ptr<Node>& node)
{
    if (node->children.size() == 0) {
        ErrorInfo err_no;
        err_no.expected_type = node->type;
        err_no.actual_value = node->data;
        return err_no;
    }
    for (int i = 0; node->children.size(); ++i) {
        auto n = std::move(node);
        if (n->accepted) {
            return getErrorInfo(n);
        }
    }
}

// PRINT DEBUG
void PrintErrorMessage(ErrorInfo info, Type enclosing_type)
{
    
}

// FILE(S) ENTRY POINT
std::unique_ptr<Node> Parser::translation_unit()
{
    // CREATE ROOT NODE
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::TRANSLATION_UNIT;

    // PROCESS TOKENS
    while (HasTokens()) {
        std::unique_ptr<Node> external_declaration_node = external_declaration();

        if(external_declaration_node->accepted) {
            self->children.push_back(std::move(external_declaration_node));
        }
        else {
            // HANDLE ERRORS
            PrintErrorMessage(getErrorInfo(external_declaration_node),external_declaration_node->children.at(0)->type);
        }
    }

    return self;
}

// EXTERNAL DECLARATION (FUNCTION OR GLOBAL VARIABLE)
std::unique_ptr<Node> Parser::external_declaration()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    std::unique_ptr<Node> function_definition_node;
    std::unique_ptr<Node> declaration_node;

    int saved_token_index = m_tokenIndex;

    function_definition_node = function_definition();
    if (function_definition_node->accepted) {
        self->accepted = true;
        self->children.push_back(std::move(function_definition_node));
    } else {
        m_tokenIndex = saved_token_index;
        declaration_node = declaration();
        if (declaration_node->accepted) {
            self->accepted = true;
            self->children.push_back(std::move(declaration_node));
        } else {
            // Show error for function_definition_node or declaration_node
        }
    }

    return self;
}

// FUNCTION DEFINITION
std::unique_ptr<Node> Parser::function_definition()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::FUNCTION_DEFINITION;

    // CHECK FOR RETURN TYPE - TODO: Handle special cases (int int, long long long, ect..)
    int saved_token_index = m_tokenIndex;
    std::unique_ptr<Node> declaration_specifier_node = declaration_specifier();
    while (declaration_specifier_node->accepted) {
        saved_token_index = m_tokenIndex;
        self->children.push_back(std::move(declaration_specifier_node));
        declaration_specifier_node = declaration_specifier();
    }
    m_tokenIndex = saved_token_index;

    // CHECK FOR FUNCTION NAME
    std::unique_ptr<Node> declarator_node = declarator();
    if (!declarator_node->accepted) {
        self->accepted = false;
    }
    self->children.push_back(std::move(declarator_node));

    saved_token_index = m_tokenIndex;
    std::unique_ptr<Node> declaration_node = declaration();
    while (declaration_node->accepted) {
        saved_token_index = m_tokenIndex;
        self->children.push_back(std::move(declaration_node));
        declaration_node = declaration();
    }
    m_tokenIndex = saved_token_index;

    // CHECK FOR FUNCTION BODY
    std::unique_ptr<Node> compound_statement_node = compound_statement();
    if (!compound_statement_node->accepted) {
        self->accepted = false;
    }
    self->children.push_back(std::move(compound_statement_node));

    return self;
}

// 
std::unique_ptr<Node> Parser::declaration_specifier()
{
    // TODO: talk to Zach about possibly altering declaration_specifier() grammar.

    // std::unique_ptr<Node> self = std::make_unique<Node>();
    // self->type = Type::DECLARATION_SPECIFIER;

    // std::unique_ptr<Node> storage_class_specifier_node;
    // std::unique_ptr<Node> type_specifier_node;
    // std::unique_ptr<Node> type_qualifier_node;

    // int saved_token_index = m_tokenIndex;
    // storage_class_specifier_node

    // return self;
}
std::unique_ptr<Node> Parser::storage_class_specifier()
{
}
std::unique_ptr<Node> Parser::type_specifier()
{
    
}
std::unique_ptr<Node> Parser::struct_or_union_specifer() {}
std::unique_ptr<Node> Parser::struct_or_union() {}
std::unique_ptr<Node> Parser::struct_declaration() {}
std::unique_ptr<Node> Parser::specifier_qualifier() {}
std::unique_ptr<Node> Parser::struct_declarator_list() {}
std::unique_ptr<Node> Parser::struct_declarator() {}

// 
std::unique_ptr<Node> Parser::declarator()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::DECLARATOR;

    int saved_token_index = m_tokenIndex;
    std::unique_ptr<Node> pointer_node = pointer();
    if (pointer_node->accepted) {
        self->children.push_back(std::move(pointer_node));
    } else {
        m_tokenIndex = saved_token_index;
    }

    std::unique_ptr<Node> direct_declarator_node = direct_declarator();
    self->accepted = direct_declarator_node->accepted;
    self->children.push_back(std::move(direct_declarator_node));

    return self;
}
std::unique_ptr<Node> Parser::pointer()
{
    
}
std::unique_ptr<Node> Parser::type_qualifier() {}
std::unique_ptr<Node> Parser::direct_declarator() {}
std::unique_ptr<Node> Parser::constant_expression() {}
std::unique_ptr<Node> Parser::logical_or_expression() {}
std::unique_ptr<Node> Parser::logical_and_expression() {}
std::unique_ptr<Node> Parser::inclusive_or_expression() {}
std::unique_ptr<Node> Parser::and_expression() {}
std::unique_ptr<Node> Parser::equality_expression() {}
std::unique_ptr<Node> Parser::relational_expression() {}
std::unique_ptr<Node> Parser::shift_expression() {}
std::unique_ptr<Node> Parser::additive_expression() {}
std::unique_ptr<Node> Parser::multiplicative_expression() {}
std::unique_ptr<Node> Parser::cast_expression() {}
std::unique_ptr<Node> Parser::unary_expression() {}
std::unique_ptr<Node> Parser::postfix_expression() {}
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
       left_parens->type = Type::LEFT_PARENTHESIS;
       right_parens->type = Type::RIGHT_PARENTHESIS;

       if (!expression_node->accepted) {
           self->accepted = false;
       } else {
           nextToken = getNextToken();
           if (nextToken.type != TokenType::CLOSE_PARENS) {
               right_parens->accepted = false;
               right_parens->data = nextToken.value;
               self->children.push_back(std::move(right_parens));
               self->accepted = false;
            } else {
                self->children.push_back(std::move(right_parens));
                self->accepted = true;
            }
       }
       self->children.push_back(std::move(left_parens));
       self->children.push_back(std::move(expression_node));
   } else {
       std::unique_ptr<Node> error_node = std::make_unique<Node>();
       error_node->type = Type::ERROR;
       error_node->data = nextToken.value;
       self->children.push_back(std::move(error_node));
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

        // Here, we save the index into our token list because we will be progressing through the input until such time as we
        // pass the last token usable by us.  We will always have to backtrack at least one token.
        int input_index = m_tokenIndex;
        Token tok = getNextToken();
        bool still_recursing = true;
        while (still_recursing) {
            if (tok.type == TokenType::COMMA) {
                std::unique_ptr<Node> comma_node = std::make_unique<Node>();
                comma_node->accepted = true;
                comma_node->type = Type::COMMA;
                comma_node->data = tok.value;

                std::unique_ptr<Node> inner_assignment_expression_node = assignment_expression();
                if (inner_assignment_expression_node->accepted) { // Both the comma and assignment-expression are valid, so we can add them to the expression node and continue
                    self->children.push_back(std::move(comma_node));
                    self->children.push_back(std::move(inner_assignment_expression_node));
                } else { // Comma was valid, but assignment-expression wasn't, so neither can be added.  And stop looping.
                    still_recursing = false;

                    // restore token index from before comma
                    m_tokenIndex = input_index;
                }

            } else { // stop looping to continue expression, and add the token (which we can't use) back on the input
                still_recursing = false;
                --m_tokenIndex;
            }

            // update input_input after correct loop pass
            input_index = m_tokenIndex;
        }
    } else {
        self->accepted = false;
        self->children.push_back(std::move(assignment_expression_node));
    }

    return self;

}
std::unique_ptr<Node> Parser::assignment_expression()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::ASSIGNMENT_EXPRESSION;

    int saved_input_index = m_tokenIndex;

    std::unique_ptr<Node> conditional_expression_node = conditional_expression();
    if (conditional_expression_node->accepted) {
        self->accepted = true;
        self->children.push_back(std::move(conditional_expression_node));
    } else {
        m_tokenIndex = saved_input_index;

        std::unique_ptr<Node> unary_expression_node = unary_expression();
        std::unique_ptr<Node> assignment_operator_node = assignment_operator();
        std::unique_ptr<Node> assignment_expression_node = assignment_expression();

        self->children.push_back(std::move(unary_expression_node));
        self->children.push_back(std::move(assignment_operator_node));
        self->children.push_back(std::move(assignment_expression_node));

        if (!unary_expression_node->accepted || !assignment_operator_node->accepted || !assignment_expression_node->accepted) {
            self->accepted = false;
        }
    }

    return self;
}

std::unique_ptr<Node> Parser::conditional_expression()
{

}

std::unique_ptr<Node> Parser::assignment_operator()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::ASSIGNMENT_OPERATOR;

    Token tok = getNextToken();

    if (tok.type == TokenType::EQUAL) {
        // =
        std::unique_ptr<Node> assignment_node = std::make_unique<Node>();
        assignment_node->accepted = true;
        assignment_node->type = Type::UNARY_ASSIGNMENT;
        assignment_node->data = tok.value;
        self->children.push_back(std::move(assignment_node));
    } else if (tok.type == TokenType::STAR_EQUAL) {
        // *=
        std::unique_ptr<Node> assign_multiply_result_node = std::make_unique<Node>();
        assign_multiply_result_node->accepted = true;
        assign_multiply_result_node->type = Type::MULTIPLICATION_ASSIGNMENT;
        assign_multiply_result_node->data = tok.value;
        self->children.push_back(std::move(assign_multiply_result_node));
    } else if (tok.type == TokenType::SLASH_EQUAL) {
        // /=
        std::unique_ptr<Node> assign_divide_result_node = std::make_unique<Node>();
        assign_divide_result_node->accepted = true;
        assign_divide_result_node->type = Type::DIVISION_ASSIGNMENT;
        assign_divide_result_node->data = tok.value;
        self->children.push_back(std::move(assign_divide_result_node));
    } else if (tok.type == TokenType::PERCENT_EQUAL) {
        // %=
        std::unique_ptr<Node> assign_modulus_result_node = std::make_unique<Node>();
        assign_modulus_result_node->accepted = true;
        assign_modulus_result_node->type = Type::MODULUS_ASSIGNMENT;
        assign_modulus_result_node->data = tok.value;
        self->children.push_back(std::move(assign_modulus_result_node));
    } else if (tok.type == TokenType::PLUS_EQUAL) {
        // +=
        std::unique_ptr<Node> assign_addition_result_node = std::make_unique<Node>();
        assign_addition_result_node->accepted = true;
        assign_addition_result_node->type = Type::ADDITION_ASSIGNMENT;
        assign_addition_result_node->data = tok.value;
        self->children.push_back(std::move(assign_addition_result_node));
    } else if (tok.type == TokenType::MINUS_EQUAL) {
        // -=
        std::unique_ptr<Node> assign_subtraction_result_node = std::make_unique<Node>();
        assign_subtraction_result_node->accepted = true;
        assign_subtraction_result_node->type = Type::SUBTRACTION_ASSIGNMENT;
        assign_subtraction_result_node->data = tok.value;
        self->children.push_back(std::move(assign_subtraction_result_node));
    } else if (tok.type == TokenType::LESS_LESS_EQUAL) {
        // <<=
        std::unique_ptr<Node> assign_left_shift_result_node = std::make_unique<Node>();
        assign_left_shift_result_node->accepted = true;
        assign_left_shift_result_node->type = Type::LEFTSHIFT_ASSIGNMENT;
        assign_left_shift_result_node->data = tok.value;
        self->children.push_back(std::move(assign_left_shift_result_node));
    } else if (tok.type == TokenType::GREATER_GREATER_EQUAL) {
        // >>=
        std::unique_ptr<Node> assign_right_shift_result_node = std::make_unique<Node>();
        assign_right_shift_result_node->accepted = true;
        assign_right_shift_result_node->type = Type::RIGHTSHIFT_ASSIGNMENT;
        assign_right_shift_result_node->data = tok.value;
        self->children.push_back(std::move(assign_right_shift_result_node));
    } else if (tok.type == TokenType::AND_EQUAL) {
        // &=
        std::unique_ptr<Node> assign_bitwise_and_result_node = std::make_unique<Node>();
        assign_bitwise_and_result_node->accepted = true;
        assign_bitwise_and_result_node->type = Type::BITWISE_AND_ASSIGNMENT;
        assign_bitwise_and_result_node->data = tok.value;
        self->children.push_back(std::move(assign_bitwise_and_result_node));
    } else if (tok.type == TokenType::XOR_EQUAL) {
        // ^=
        std::unique_ptr<Node> assign_xor_result_node = std::make_unique<Node>();
        assign_xor_result_node->accepted = true;
        assign_xor_result_node->type = Type::XOR_ASSIGNMENT;
        assign_xor_result_node->data = tok.value;
        self->children.push_back(std::move(assign_xor_result_node));
    } else if (tok.type == TokenType::OR_EQUAL) {
        // |=
        std::unique_ptr<Node> assign_bitwise_or_result_node = std::make_unique<Node>();
        assign_bitwise_or_result_node->accepted = true;
        assign_bitwise_or_result_node->type = Type::BITWISE_OR_ASSIGNMENT;
        assign_bitwise_or_result_node->data = tok.value;
        self->children.push_back(std::move(assign_bitwise_or_result_node));
    } else { // Error, token does not match expected terminal.
        std::unique_ptr<Node> error_node = std::make_unique<Node>();
        error_node->type = Type::ERROR;
        error_node->data = tok.value;
        self->children.push_back(std::move(error_node));
        self->accepted = false;
    }

    return self;
}

std::unique_ptr<Node> Parser::unary_operator()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::UNARY_OPERATOR;

    Token tok = getNextToken();

    if (tok.type == TokenType::AND) {
        std::unique_ptr<Node> unary_and_node = std::make_unique<Node>();
        unary_and_node->accepted = true;
        unary_and_node->type = Type::UNARY_AND;
        unary_and_node->data = tok.value;
        self->children.push_back(std::move(unary_and_node));
    } else if (tok.type == TokenType::STAR) {
        std::unique_ptr<Node> pointer_op_node = std::make_unique<Node>();
        pointer_op_node->accepted = true;
        pointer_op_node->type = Type::POINTER_OP;
        pointer_op_node->data = tok.value;
        self->children.push_back(std::move(pointer_op_node));
    } else if (tok.type == TokenType::PLUS) {
        std::unique_ptr<Node> unary_positive_node = std::make_unique<Node>();
        unary_positive_node->accepted = true;
        unary_positive_node->type = Type::UNARY_POSITIVE;
        unary_positive_node->data = tok.value;
        self->children.push_back(std::move(unary_positive_node));
    } else if (tok.type == TokenType::MINUS) {
        std::unique_ptr<Node> unary_negative_node = std::make_unique<Node>();
        unary_negative_node->accepted = true;
        unary_negative_node->type = Type::UNARY_NEGATIVE;
        unary_negative_node->data = tok.value;
        self->children.push_back(std::move(unary_negative_node));
    } else if (tok.type == TokenType::TILDE) {
        std::unique_ptr<Node> bitwise_not_node = std::make_unique<Node>();
        bitwise_not_node->accepted = true;
        bitwise_not_node->type = Type::BITWISE_NOT;
        bitwise_not_node->data = tok.value;
        self->children.push_back(std::move(bitwise_not_node));
    } else if (tok.type == TokenType::NOT) {
        std::unique_ptr<Node> unary_not_node = std::make_unique<Node>();
        unary_not_node->accepted = true;
        unary_not_node->type = Type::UNARY_NOT;
        unary_not_node->data = tok.value;
        self->children.push_back(std::move(unary_not_node));
    } else {
        std::unique_ptr<Node> error_node = std::make_unique<Node>();
        error_node->type = Type::ERROR;
        error_node->data = tok.value;
        self->children.push_back(std::move(error_node));
        self->accepted = false;
    }

    return self;
}
std::unique_ptr<Node> Parser::type_name() {}
std::unique_ptr<Node> Parser::parameter_type_list() {}
std::unique_ptr<Node> Parser::parameter_list() {}
std::unique_ptr<Node> Parser::parameter_declaration() {}
std::unique_ptr<Node> Parser::abstract_declarator() {}
std::unique_ptr<Node> Parser::direct_abstract_declarator() {}
std::unique_ptr<Node> Parser::enum_specifer() {}
std::unique_ptr<Node> Parser::enumerator_list() {}
std::unique_ptr<Node> Parser::enumerator() {}
std::unique_ptr<Node> Parser::typedef_name() {}
std::unique_ptr<Node> Parser::declaration() {}
std::unique_ptr<Node> Parser::init_declarator() {}
std::unique_ptr<Node> Parser::initializer() {}
std::unique_ptr<Node> Parser::initializer_list() {}
std::unique_ptr<Node> Parser::compound_statement() {}
std::unique_ptr<Node> Parser::statement() {}
std::unique_ptr<Node> Parser::labeled_statement() {}
std::unique_ptr<Node> Parser::expression_statement() {}
std::unique_ptr<Node> Parser::selection_statement() {}
std::unique_ptr<Node> Parser::iteration_statement() {}


// TODO: test this.  I did it weird. (iterative instead of recursive to wrap my head around left-refactoring)
std::unique_ptr<Node> Parser::jump_statement()
{
    std::unique_ptr<Node> self = std::make_unique<Node>();
    self->type = Type::JUMP_STATEMENT;

    Token tok = getNextToken();

    if (tok.type == TokenType::GOTO) {
        std::unique_ptr<Node> goto_node = std::make_unique<Node>();
        goto_node->accepted = true;
        goto_node->type = Type::GOTO;
        goto_node->data = tok.value;
        self->children.push_back(std::move(goto_node));

        tok = getNextToken();
        if (tok.type == TokenType::ID) {
            std::unique_ptr<Node> goto_label = std::make_unique<Node>();
            goto_label->accepted = true;
            goto_node->type = Type::GOTO;
            goto_node->data = tok.value;

            self->children.push_back(std::move(goto_label));
        } else {
            self->accepted = false;
        }
    } else if (tok.type == TokenType::CONTINUE) {
        std::unique_ptr<Node> continue_node = std::make_unique<Node>();
        continue_node->accepted = true;
        continue_node->type = Type::CONTINUE;
        continue_node->data = tok.value;
        self->children.push_back(std::move(continue_node));
    } else if (tok.type == TokenType::BREAK) {
        std::unique_ptr<Node> break_node = std::make_unique<Node>();
        break_node->accepted = true;
        break_node->type = Type::BREAK;
        break_node->data = tok.value;
        self->children.push_back(std::move(break_node));
    } else {
        std::unique_ptr<Node> error_node = std::make_unique<Node>();
        error_node->type = Type::ERROR;
        error_node->data = tok.value;
        self->children.push_back(std::move(error_node));
        self->accepted = false;
    }

    // Next input should be semi-colon, for all possible productions
    tok = getNextToken();
    if (tok.type == TokenType::SEMICOLON) {
        std::unique_ptr<Node> semicolon_node = std::make_unique<Node>();
        semicolon_node->accepted = true;
        semicolon_node->type = Type::STATEMENT_END;
        semicolon_node->data = tok.value;
        self->children.push_back(std::move(semicolon_node));
    } else {
        self->accepted = false;
    }

    return self;
}