#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct LEXER_STRUCT
{
    char c; // current character.
    unsigned int i;
    char* contents; // the source code to lex.
} lexer_T;

lexer_T* init_lexer(char* contents); // init lexer.

void lexer_advance(lexer_T* lexer); // move pointer to the next character in the content.

void lexer_skip_whitespace(lexer_T* lexer); // skip the white space.

void lexer_skip_comment(lexer_T* lexer); // skip the comment.

token_T* lexer_get_next_token(lexer_T* lexer); // call this function to get the next token in the contents.

token_T* lexer_collect_string(lexer_T* lexer); // define how to parse string.

token_T* lexer_collect_id(lexer_T* lexer); // define how to parse id.

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token);

char* lexer_get_current_char_as_string(lexer_T* lexer); // return current character as string.

#endif