#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    token_T* prev_token = parser->current_token;

    parser->scope = init_scope();

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
            "SyntaxError: Unexpected token '%s' (line %d)\n",
            parser->current_token->value,
            parser->lexer->line_n
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser, scope_T* scope) // main entry point of the parser. return the whole AST tree of the entire source code when we parse.
{
    return parser_parse_statements(parser, scope);
}

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope) // parsing single statement
{
    switch (parser->current_token->type)
    {
        // if parser current token type is TOKEN ID, parse token id
        case TOKEN_ID: return parser_parse_id(parser, scope);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope) // parsing list of statement
{
    AST_T* compound = init_ast(AST_COMPOUND); // create compound type ast node
    compound->scope = scope;
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*)); // alocating memory for the compound list

    AST_T* ast_statement = parser_parse_statement(parser, scope); // parsing statement
    ast_statement->scope = scope;
    compound->compound_value[0] = ast_statement; // adding the statement to the begining list
    compound->compound_size += 1;

    // parse another statement if COLON
    while (parser->current_token->type == TOKEN_COLON)
    {
        parser_eat(parser, TOKEN_COLON);

        AST_T* ast_statement = parser_parse_statement(parser, scope); // parse another statement

        if (ast_statement)
        {
            compound->compound_size += 1; // increase the size

            // realocating the memory
            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct AST_STRUCT)
            );
            compound->compound_value[compound->compound_size-1] = ast_statement; // put the new statement to the last statement of the list
        }
    }

    return compound;
}

AST_T* parser_parse_expr(parser_T* parser, scope_T* scope)
{
    // printf("parser parse expr\n");
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser, scope);
        case TOKEN_INT: return parser_parse_int(parser, scope);
        case TOKEN_ID: return parser_parse_id(parser, scope);
        case TOKEN_SEMI: return parser_parse_list(parser, scope);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_factor(parser_T* parser, scope_T* scope)
{
    return 0;
}

AST_T* parser_parse_term(parser_T* parser, scope_T* scope)
{
    return 0;
}

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope) // return AST node type of function call
{
    // printf("%s\n", parser->prev_token->value); // printf the prev token value
    AST_T* function_call = init_ast(AST_FUNCTION_CALL);

    function_call->function_call_name = parser->prev_token->value;

    parser_eat(parser, TOKEN_LPAREN); // eat TOKEN LPAREN

    function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT*)); // alocating memory for the function call list
    if (parser->current_token->type != TOKEN_RPAREN) // if current token type is not Right Paren means not exit
    {
        // printf("%s\n", parser->current_token->value);
        AST_T* ast_expr = parser_parse_expr(parser, scope); // parsing statement
        function_call->function_call_arguments[0] = ast_expr; // adding the statement to the begining list
        function_call->function_call_arguments_size += 1;
    }

    // parse another function call arguments if COMMA
    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);

        // printf("%s\n", parser->current_token->value);

        AST_T* ast_expr = parser_parse_expr(parser, scope); // parse the expressions
        function_call->function_call_arguments_size += 1; // increase the arguments size

        // realocating the memory for the function arguments list
        function_call->function_call_arguments = realloc(
            function_call->function_call_arguments,
            function_call->function_call_arguments_size * sizeof(struct AST_STRUCT)
        );
        function_call->function_call_arguments[function_call->function_call_arguments_size-1] = ast_expr; // add the new expression to the last index of the list
    }

    parser_eat(parser, TOKEN_RPAREN); // eat TOKEN RPAREN

    function_call->scope = scope;

    return function_call;
}

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_VARIABLE_DEFINITION); // create ast node that will return

    parser_eat(parser, TOKEN_ID); // expecting v
    char * variable_name = parser->current_token->value;
    ast->variable_definition_variable_name = calloc(
        strlen(variable_name) + 1,
        sizeof(char)
    );
    strcpy(ast->variable_definition_variable_name, variable_name); // saving the variable name

    parser_eat(parser, TOKEN_ID); // expecting v name
    parser_eat(parser, TOKEN_EQUALS); // expecting equals
    AST_T* variable_definition_value = parser_parse_expr(parser, scope); // expecting value
    ast->variable_definition_value = variable_definition_value;

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_FUNCTION_DEFINITION);
    // printf("Parse function\n");
    parser_eat(parser, TOKEN_ID); // func

    char* function_name = parser->current_token->value;
    ast->function_definition_name = calloc(
        strlen(function_name) + 1,
        sizeof(char)
    );
    strcpy(ast->function_definition_name, function_name);

    parser_eat(parser, TOKEN_ID); // func name
    parser_eat(parser, TOKEN_LPAREN);
    
    if (parser->current_token->type != TOKEN_RPAREN)
    {
        ast->function_definition_args = calloc(1, sizeof(struct AST_STRUCT*));

        AST_T* arg = parser_parse_variable(parser, scope);
        ast->function_definition_args_size += 1;
        ast->function_definition_args[ast->function_definition_args_size-1] = arg;

        while (parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA);

            ast->function_definition_args_size += 1;

            ast->function_definition_args =
                realloc(
                    ast->function_definition_args,
                    ast->function_definition_args_size * sizeof(struct AST_STRUCT*)
                );

            AST_T* arg = parser_parse_variable(parser, scope);
            ast->function_definition_args[ast->function_definition_args_size-1] = arg;
        }
    }

    parser_eat(parser, TOKEN_RPAREN);
    parser_eat(parser, TOKEN_LBRACE);

    ast->function_definition_body = parser_parse_statements(parser, scope);

    parser_eat(parser, TOKEN_RBRACE);

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_list_definition(parser_T* parser, scope_T* scope)
{
    // printf("found the list definition!!!\n");
    AST_T* ast = init_ast(AST_LIST_DEFINITION);
    parser_eat(parser, TOKEN_ID);
    char* list_name = parser->current_token->value;
    ast->list_definition_name = calloc(
        strlen(list_name) + 1,
        sizeof(char)
    );
    strcpy(ast->list_definition_name, list_name);

    parser_eat(parser, TOKEN_ID);
    parser_eat(parser, TOKEN_LPAREN);
    // printf("value: %s\n", parser->current_token->value);

    if (parser->current_token->type != TOKEN_RPAREN)
    {
        ast->list_definition_args = calloc(1, sizeof(struct AST_STRUCT*));

        AST_T* arg = parser_parse_expr(parser, scope);
        ast->list_definition_args_size += 1;
        ast->list_definition_args[ast->list_definition_args_size-1] = arg;

        while (parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA);

            ast->list_definition_args_size += 1;

            ast->list_definition_args =
                realloc(
                    ast->list_definition_args,
                    ast->list_definition_args_size * sizeof(struct AST_STRUCT*)
                );
            // printf("value: %s\n", parser->current_token->value);
            AST_T* arg = parser_parse_expr(parser, scope);
            ast->list_definition_args[ast->list_definition_args_size-1] = arg;
        }
    }

    parser_eat(parser, TOKEN_RPAREN);

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_variable(parser_T* parser, scope_T* scope) // return AST node type of variable
{
    char* token_value = parser->current_token->value;

    parser_eat(parser, TOKEN_ID); // v name or function call name

    if (parser->current_token->type == TOKEN_MINUS)
    {
        // if ~ list call
        return parser_parse_list(parser, scope);
    }

    if (parser->current_token->type == TOKEN_LPAREN)
        // if [ -> function call
        return parser_parse_function_call(parser, scope);

    // assuming that it's a variable
    AST_T* ast_variable = init_ast(AST_VARIABLE);  // create a node for it
    ast_variable->variable_name = token_value; // assign a variable name

    ast_variable->scope = scope;

    return ast_variable;
}

AST_T* parser_parse_string(parser_T* parser, scope_T* scope) // return AST node type of string{
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    ast_string->scope = scope;

    return ast_string;
}

AST_T* parser_parse_int(parser_T* parser, scope_T* scope) // return AST node type of string
{
    char* endPtr;
    long long int int_value = strtoll(parser->current_token->value, &endPtr, 10);
    
    parser_eat(parser, TOKEN_INT);
    AST_T* ast_int = init_ast(AST_INT);
    ast_int->int_value = int_value;

    ast_int->scope = scope;
    
    // printf("parser parse int %d\n", int_value);
    return ast_int;
}

AST_T* parser_parse_list(parser_T* parser, scope_T* scope)
{
    char* token_value = parser->prev_token->value;

    // assuming that it's a list.
    AST_T* ast_list = init_ast(AST_LIST);  // create a node for it
    ast_list->list_name = token_value; // assign a list name
    parser_eat(parser, TOKEN_MINUS);
    parser_eat(parser, TOKEN_RP);

    AST_T* list_index = parser_parse_expr(parser, scope);
    ast_list->list_index = list_index;

    ast_list->scope = scope;

    return ast_list;
}

AST_T* parser_parse_id(parser_T* parser, scope_T* scope)
{
    if (strcmp(parser->current_token->value, "v") == 0)
    {
        // if parser current token value is v define a variable
        return parser_parse_variable_definition(parser, scope);
    }
    if (strcmp(parser->current_token->value, "func") == 0)
    {
        return parser_parse_function_definition(parser, scope);
    }
    if (strcmp(parser->current_token->value, "list") == 0)
    {
        return parser_parse_list_definition(parser, scope);
    }
    else
    {
        return parser_parse_variable(parser, scope);
    }
}
