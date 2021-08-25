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
    return parser_parse_statements();
}

AST_T* parser_parse_statement(parser_T* parser) // parsing single statement
{
    switch (parser->token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser);
    }
}

AST_T* parser_parse_statements(parser_T* parser) // parsing list of statement
{
    AST_T* compound = init_ast(AST_COMPOUND); // create compound type ast node
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement(parser);
    compound->compound_value[0] = ast_statement;

    // parse another statement if SEMI
    while (parser->current_token->type = TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);

        AST_T* ast_statement = parser_parse_statement(parser); // parse another statement
        compound->compound_size += 1; // increase the size

        // realocating the memory
        compound->compound_value = realloc(
            compound->compound_value,
            compound->compound_size * sizeof(struct AST_STRUCT)
        );
        compound->compound_value[compound->compound_size-1] = ast_statement; // put the new statement to the last statement of the list
    }

    return compound;
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

AST_T* parser_parse_variable_definition(parser_T* parser)
{
    
}

AST_T* parser_parse_variable(parser_T* parser) // return AST node type of variable
{

}

AST_T* parser_parse_string(parser_T* parser) // return AST node type of string{
{
}

AST_T* parser_parse_id(parser_T* parser)
{
    if (strcmp(parser->current_token->value, "v") == 0)
    {
        return parser_parse_variable_definition(parser);
    }
    else
    {
        return parser_parse_variable(parser);
    }
}
