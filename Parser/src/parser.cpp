#include "parser.h"

#include <memory>
#include <iostream>

Parser::Parser() : m_preprocessor(), m_root(), m_errors(), m_tokens()
{
    m_root = std::make_shared<Node>();
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

bool Parser::BuildTree() {
    m_root = translation_unit();
}

struct ErrorInfo {
    Type expected_type;
    std::string actual_value;
};

ErrorInfo getErrorInfo(std::shared_ptr<Node>& node)
{
    if (node->children.size() == 0) {
        ErrorInfo err_no;
        err_no.expected_type = node->type;
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

// FILE(S) ENTRY POINT
std::shared_ptr<Node> Parser::translation_unit()
{
    // CREATE ROOT NODE
    std::shared_ptr<Node> self = std::make_shared<Node>();
    self->type = Type::TRANSLATION_UNIT;

    // PROCESS TOKENS
    while (HasTokens()) {
        std::shared_ptr<Node> external_declaration_node = external_declaration();

        if(external_declaration_node->accepted) {
            self->addChild((external_declaration_node));
        }
        // HANDLE ERRORS
        else {
            ErrorInfo ei = getErrorInfo(external_declaration_node);
            // could be function or declaration (global variable)
            if (external_declaration_node->children.at(0)->type == Type::FUNCTION_DEFINITION) {
                //Print("In Function")
            } else if (external_declaration_node->children.at(0)->type == Type::DECLARATION) {
                //Print("In declaration")
            }
        }
    }

    return self;
}

// EXTERNAL DECLARATION (FUNCTION OR GLOBAL VARIABLE)
std::shared_ptr<Node> Parser::external_declaration()
{
    std::shared_ptr<Node> self = std::make_shared<Node>();
    std::shared_ptr<Node> function_definition_node;
    std::shared_ptr<Node> declaration_node;

    int saved_token_index = m_tokenIndex;

    function_definition_node = function_definition();
    if (function_definition_node->accepted) {
        self->addChild((function_definition_node));
    } else {
        m_tokenIndex = saved_token_index;
        declaration_node = declaration();
        if (declaration_node->accepted) {
            self->addChild((declaration_node));
        } else {
            // Show error for function_definition_node or declaration_node
        }
    }

    return self;
}

// FUNCTION DEFINITION
std::shared_ptr<Node> Parser::function_definition()
{
    std::shared_ptr<Node> self = std::make_shared<Node>();
    self->type = Type::FUNCTION_DEFINITION;

    // CHECK FOR RETURN TYPE - TODO: Handle special cases (int int, long long long, ect..)
    int saved_token_index = m_tokenIndex;
    std::shared_ptr<Node> declaration_specifier_node = declaration_specifier();
    while (declaration_specifier_node->accepted) {
        saved_token_index = m_tokenIndex;
        self->addChild((declaration_specifier_node));
        declaration_specifier_node = declaration_specifier();
    }
    m_tokenIndex = saved_token_index;

    // CHECK FOR FUNCTION NAME
    std::shared_ptr<Node> declarator_node = declarator();
    if (!declarator_node->accepted) {
        self->accepted = false;
    }
    self->addChild((declarator_node));

    saved_token_index = m_tokenIndex;
    std::shared_ptr<Node> declaration_node = declaration();
    while (declaration_node->accepted) {
        saved_token_index = m_tokenIndex;
        self->addChild((declaration_node));
        declaration_node = declaration();
    }
    m_tokenIndex = saved_token_index;

    // CHECK FOR FUNCTION BODY
    std::shared_ptr<Node> compound_statement_node = compound_statement();
    if (!compound_statement_node->accepted) {
        self->accepted = false;
    }
    self->addChild((compound_statement_node));

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
std::shared_ptr<Node> Parser::storage_class_specifier()
{
}
std::shared_ptr<Node> Parser::type_specifier()
{
    
}
std::shared_ptr<Node> Parser::struct_or_union_specifer() {}
std::shared_ptr<Node> Parser::struct_or_union() {}
std::shared_ptr<Node> Parser::struct_declaration() {}
std::shared_ptr<Node> Parser::specifier_qualifier() {}
std::shared_ptr<Node> Parser::struct_declarator_list() {}
std::shared_ptr<Node> Parser::struct_declarator() {}

// 
std::shared_ptr<Node> Parser::declarator()
{
    std::shared_ptr<Node> self = std::make_shared<Node>();
    self->type = Type::DECLARATOR;

    int saved_token_index = m_tokenIndex;
    std::shared_ptr<Node> pointer_node = pointer();
    if (pointer_node->accepted) {
        self->addChild((pointer_node));
    } else {
        m_tokenIndex = saved_token_index;
    }

    std::shared_ptr<Node> direct_declarator_node = direct_declarator();
    self->addChild((direct_declarator_node));

    return self;
}
std::shared_ptr<Node> Parser::pointer()
{
    
}
std::shared_ptr<Node> Parser::type_qualifier() {}
std::shared_ptr<Node> Parser::direct_declarator() {}
std::shared_ptr<Node> Parser::constant_expression() {}
std::shared_ptr<Node> Parser::logical_or_expression() {}
std::shared_ptr<Node> Parser::logical_and_expression() {}
std::shared_ptr<Node> Parser::inclusive_or_expression() {}
std::shared_ptr<Node> Parser::and_expression() {}
std::shared_ptr<Node> Parser::equality_expression() {}
std::shared_ptr<Node> Parser::relational_expression() {}
std::shared_ptr<Node> Parser::shift_expression() {}
std::shared_ptr<Node> Parser::additive_expression() {}
std::shared_ptr<Node> Parser::multiplicative_expression() {}
std::shared_ptr<Node> Parser::cast_expression() {}
std::shared_ptr<Node> Parser::unary_expression() {}
std::shared_ptr<Node> Parser::postfix_expression() {}
std::shared_ptr<Node> Parser::primary_expression()
{
    /*
    * Primary expression is the most basic form of expression, consisting of an identifier, string-literal, or numeric-constant.
    * It can be surrounded by any number of parentheses?
    */
   std::shared_ptr<Node> self = std::make_shared<Node>();
   self->type = Type::PRIMARY_EXPRESSION;

   // primary_expression is NOT an error handling node, so we don't save m_tokenIndex here prior to getNextToken
   Token nextToken = getNextToken();
   if (nextToken.type == TokenType::ID) {
       std::shared_ptr<Node> identifier_node = std::make_shared<Node>();
       identifier_node->type = Type::IDENTIFIER;
       identifier_node->accepted = true;
       // TODO: identifier_node.value or .data, etc should 'point' to an entry in a symbol table describing the identifier
       // For now, just the name
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
std::shared_ptr<Node> Parser::expression()
{
    std::shared_ptr<Node> self = std::make_shared<Node>();
    self->type = Type::EXPRESSION;

    std::shared_ptr<Node> assignment_expression_node = assignment_expression();
    if (assignment_expression_node->accepted) {
        self->addChild((assignment_expression_node));

        // Here, we save the index into our token list because we will be progressing through the input until such time as we
        // pass the last token usable by us.  We will always have to backtrack at least one token.
        int input_index = m_tokenIndex;
        Token tok = getNextToken();
        bool still_recursing = true;
        while (still_recursing) {
            if (tok.type == TokenType::COMMA) {
                std::shared_ptr<Node> comma_node = std::make_shared<Node>();
                comma_node->accepted = true;
                comma_node->type = Type::COMMA;
                comma_node->data = tok.value;

                std::shared_ptr<Node> inner_assignment_expression_node = assignment_expression();
                if (inner_assignment_expression_node->accepted) { // Both the comma and assignment-expression are valid, so we can add them to the expression node and continue
                    self->addChild((comma_node));
                    self->addChild((inner_assignment_expression_node));
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
        self->addChild((assignment_expression_node));
    }

    return self;

}
std::shared_ptr<Node> Parser::assignment_expression()
{
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

        if (!unary_expression_node->accepted || !assignment_operator_node->accepted || !assignment_expression_node->accepted) {
            self->accepted = false;
        }
    }

    return self;
}

std::shared_ptr<Node> Parser::conditional_expression()
{

}

std::shared_ptr<Node> Parser::assignment_operator()
{
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
        std::shared_ptr<Node> assign_multiply_result_node = std::make_shared<Node>();
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
        std::shared_ptr<Node> assign_addition_result_node = std::make_shared<Node>();
        assign_addition_result_node->accepted = true;
        assign_addition_result_node->type = Type::ADDITION_ASSIGNMENT;
        assign_addition_result_node->data = tok.value;
        self->addChild((assign_addition_result_node));
    } else if (tok.type == TokenType::MINUS_EQUAL) {
        // -=
        std::shared_ptr<Node> assign_subtraction_result_node = std::make_shared<Node>();
        assign_subtraction_result_node->accepted = true;
        assign_subtraction_result_node->type = Type::SUBTRACTION_ASSIGNMENT;
        assign_subtraction_result_node->data = tok.value;
        self->addChild((assign_subtraction_result_node));
    } else if (tok.type == TokenType::LESS_LESS_EQUAL) {
        // <<=
        std::shared_ptr<Node> assign_left_shift_result_node = std::make_shared<Node>();
        assign_left_shift_result_node->accepted = true;
        assign_left_shift_result_node->type = Type::LEFTSHIFT_ASSIGNMENT;
        assign_left_shift_result_node->data = tok.value;
        self->addChild((assign_left_shift_result_node));
    } else if (tok.type == TokenType::GREATER_GREATER_EQUAL) {
        // >>=
        std::shared_ptr<Node> assign_right_shift_result_node = std::make_shared<Node>();
        assign_right_shift_result_node->accepted = true;
        assign_right_shift_result_node->type = Type::RIGHTSHIFT_ASSIGNMENT;
        assign_right_shift_result_node->data = tok.value;
        self->addChild((assign_right_shift_result_node));
    } else if (tok.type == TokenType::AND_EQUAL) {
        // &=
        std::shared_ptr<Node> assign_bitwise_and_result_node = std::make_shared<Node>();
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
        std::shared_ptr<Node> assign_bitwise_or_result_node = std::make_shared<Node>();
        assign_bitwise_or_result_node->accepted = true;
        assign_bitwise_or_result_node->type = Type::BITWISE_OR_ASSIGNMENT;
        assign_bitwise_or_result_node->data = tok.value;
        self->addChild((assign_bitwise_or_result_node));
    } else { // Error, token does not match expected terminal.
        std::shared_ptr<Node> error_node = std::make_shared<Node>();
        error_node->type = Type::ERROR;
        error_node->data = tok.value;
        error_node->accepted = false;
        self->addChild((error_node));
    }

    return self;
}

std::shared_ptr<Node> Parser::unary_operator()
{
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
        pointer_op_node->type = Type::POINTER_OP;
        pointer_op_node->data = tok.value;
        self->addChild((pointer_op_node));
    } else if (tok.type == TokenType::PLUS) {
        std::shared_ptr<Node> unary_positive_node = std::make_shared<Node>();
        unary_positive_node->accepted = true;
        unary_positive_node->type = Type::UNARY_POSITIVE;
        unary_positive_node->data = tok.value;
        self->addChild((unary_positive_node);
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
std::shared_ptr<Node> Parser::enum_specifer() {}
std::shared_ptr<Node> Parser::enumerator_list() {}
std::shared_ptr<Node> Parser::enumerator() {}
std::shared_ptr<Node> Parser::typedef_name() {}
std::shared_ptr<Node> Parser::declaration() {}
std::shared_ptr<Node> Parser::init_declarator() {}
std::shared_ptr<Node> Parser::initializer() {}
std::shared_ptr<Node> Parser::initializer_list() {}
std::shared_ptr<Node> Parser::compound_statement() {}
std::shared_ptr<Node> Parser::statement() {}
std::shared_ptr<Node> Parser::labeled_statement() {}
std::shared_ptr<Node> Parser::expression_statement() {}
std::shared_ptr<Node> Parser::selection_statement() {}
std::shared_ptr<Node> Parser::iteration_statement() {}


// TODO: test this.  I did it weird. (iterative instead of recursive to wrap my head around left-refactoring)
std::shared_ptr<Node> Parser::jump_statement()
{
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