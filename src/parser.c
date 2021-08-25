#include "include/parser.h"
#include <stdio.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);

    return parser;
}

void parser_eat(parser_T* parser, int token_type) // get a expected token
{
    if (parser->current_token->type == token_type)
    {
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf(
            "[Parser ERROR] Unexpected token '%s', whith type %d",
            parser->current_token->value,
            parser->current_token->type
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser) // main entry point of the parser. return the whole AST tree of the entire source code when we parse.
{

}

AST_T* parser_parse_statement(parser_T* parser) // parsing single statement
{

}

AST_T* parser_parse_statements(parser_T* parser) // parsing list of statement
{

}

AST_T* parser_parse_expr(parser_T* parser)
{

}

AST_T* parser_parse_factor(parser_T* parser)
{

}

AST_T* parser_parse_term(parser_T* parser)
{

}

AST_T* parser_parse_function_call(parser_T* parser) // return AST node type of function call
{

}

AST_T* parser_parse_variable(parser_T* parser) // return AST node type of variable
{

}

AST_T* parser_parse_string(parser_T* parser) // return AST node type of string{
{
}
