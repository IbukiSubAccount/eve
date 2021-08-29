#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum
    {
        TOKEN_ID, // identifier
        TOKEN_EQUALS, // =
        TOKEN_STRING, // "abc"
        TOKEN_SEMI, // ;
        TOKEN_LPAREN, // (
        TOKEN_RPAREN, // )
        TOKEN_COMMA, // ,
        TOKEN_PLUS, // +
        TOKEN_EOF
    } type;

    char* value;
} token_T;

token_T* init_token(int type, char* value); // init token

#endif