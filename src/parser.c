#include "include/parser.h"
#include <stdio.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    token_T* prev_token = parser->current_token;

    return parser;
}

void parser_eat(parser_T* parser, int token_type) // get a expected token
{
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token; // save current token in the prev token
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
    return parser_parse_statements(parser);
}

AST_T* parser_parse_statement(parser_T* parser) // parsing single statement
{
    switch (parser->current_token->type)
    {
        // if parser current token type is TOKEN ID, parse token id
        case TOKEN_ID: return parser_parse_id(parser);
    }
}

AST_T* parser_parse_statements(parser_T* parser) // parsing list of statement
{
    AST_T* compound = init_ast(AST_COMPOUND); // create compound type ast node
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*)); // alocating memory for the compound list

    AST_T* ast_statement = parser_parse_statement(parser); // parsing statement
    compound->compound_value[0] = ast_statement; // adding the statement to the begining list

    // parse another statement if SEMI
    while (parser->current_token->type == TOKEN_SEMI)
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
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser);
    }
}

AST_T* parser_parse_factor(parser_T* parser)
{

}

AST_T* parser_parse_term(parser_T* parser)
{

}

AST_T* parser_parse_function_call(parser_T* parser) // return AST node type of function call
{
    AST_T* function_call = init_ast(AST_FUNCTION_CALL);
    function_call->function_call_name = parser->prev_token->value;

    function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT*)); // alocating memory for the function call list

    AST_T* ast_expr = parser_parse_expr(parser); // parsing statement
    function_call->function_call_arguments[0] = ast_expr; // adding the statement to the begining list

    // parse another function call arguments if COMMA
    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);

        AST_T* ast_expr = parser_parse_expr(parser); // parse the expressions
        function_call->function_call_arguments_size += 1; // increase the arguments size

        // realocating the memory for the function arguments list
        function_call->function_call_arguments = realloc(
            function_call->function_call_arguments,
            function_call->function_call_arguments_size * sizeof(struct AST_STRUCT)
        );
        function_call->function_call_arguments[function_call->function_call_arguments_size-1] = ast_expr; // add the new expression to the last index of the list
    }

    // printf("%s\n", parser->prev_token->value); // printf the prev token value

    return function_call;
}

AST_T* parser_parse_variable_definition(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID); // expecting v
    char * variable_definition_variable_name = parser->current_token->value; // saving the variable name
    parser_eat(parser, TOKEN_ID); // expecting v name
    parser_eat(parser, TOKEN_EQUALS); // expecting equals
    AST_T* variable_definition_value = parser_parse_expr(parser); // expecting value

    AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION); // create ast node that will return
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;

    return variable_definition;
}

AST_T* parser_parse_variable(parser_T* parser) // return AST node type of variable
{
    char* token_value = parser->current_token->value;

    parser_eat(parser, TOKEN_ID); // v name or function call name

    if (parser->current_token->type == TOKEN_LPAREN)
        // if ( -> function call
        return parser_parse_function_call(parser);

    // assuming that it's a variable
    AST_T* ast_variable = init_ast(AST_VARIABLE);  // create a node for it
    ast_variable->variable_name = token_value; // assign a variable name

    return ast_variable;
}

AST_T* parser_parse_string(parser_T* parser) // return AST node type of string{
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    return ast_string;
}

AST_T* parser_parse_id(parser_T* parser)
{
    if (strcmp(parser->current_token->value, "v") == 0)
    {
        // if parser current token value is v define a variable
        return parser_parse_variable_definition(parser);
    }
    else
    {
        return parser_parse_variable(parser);
    }
}
