#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"

int main(int argc, char* argv[])
{
    lexer_T* lexer = init_lexer(
        "v name = \"Ibuki Yoshida\";\n"
        "eve(name);\n"
    );

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);

    return 0;
}