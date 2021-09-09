#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "lexer.h"
#include "scope.h"

typedef struct PARSER_STRUCT
{
    lexer_T* lexer;
    token_T* current_token;
    token_T* prev_token;
    scope_T* scope;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_eat(parser_T* parser, int token_type); // get a expected token

AST_T* parser_parse(parser_T* parser, scope_T* scope); // main entry point of the parser. return the whole AST tree of the entire source code when we parse.

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope); // parsing single statement

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope); // parsing list of statement

AST_T* parser_parse_expr(parser_T* parser, scope_T* scope);

AST_T* parser_parse_factor(parser_T* parser, scope_T* scope);

AST_T* parser_parse_term(parser_T* parser, scope_T* scope);

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope); // return AST node type of function call

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope); // return AST node type of variable definition

AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope);

AST_T* parser_parse_variable(parser_T* parser, scope_T* scope); // return AST node type of variable

AST_T* parser_parse_input(parser_T* parser, scope_T* scope); //return AST node type of input variable

AST_T* parser_parse_string(parser_T* parser, scope_T* scope); // return AST node type of string

AST_T* parser_parse_int(parser_T* parser, scope_T* scope); // return AST node type of string

AST_T* parser_parse_id(parser_T* parser, scope_T* scope);

#endif