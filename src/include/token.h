#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum
    {
        TOKEN_ID, // identifier
        TOKEN_EQUALS, // =
        TOKEN_STRING, // "abc"
        TOKEN_INT, // 123
        TOKEN_SEMI, // ;
        TOKEN_COLON, // :
        TOKEN_LPAREN, // [
        TOKEN_RPAREN, // ]
        TOKEN_LBRACE, // {
        TOKEN_RBRACE, // }
        TOKEN_COMMA, // ,
        TOKEN_RP, // >
        TOKEN_MINUS, // -
        TOKEN_EOF
    } type;

    char* value;
} token_T;

token_T* init_token(int type, char* value); // init token

#endif