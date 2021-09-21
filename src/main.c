#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"
#include "include/shell.h"
#include <string.h>

int main(int argc, char* argv[])
{   
    if (argc < 2) 
    {
        // print_help();
        printf("Eve Programming Language Shell 0.0.1\n");
        printf("Type 'help', 'version' for more information.\n");
        while (1)
        {
            char* str = get_shell_str(">>> ");

            if (strcmp(str, "help") == 0)
            {
                printf("Usage to use files: eve <filename>\n");
                printf("How to use Eve (Docs) 🧐：https://eveofficial.herokuapp.com/docs\n");
            }
            else
            if (strcmp(str, "version") == 0)
            {
                printf("Eve 0.0.1\n");
                
            }
            else
            {
            lexer_T* lexer_shell = init_lexer(str);
            parser_T* parser_shell = init_parser(lexer_shell);
            AST_T* root_shell = parser_parse(parser_shell, parser_shell->scope);
            visitor_T* visitor_shell = init_visitor();
            visitor_visit(visitor_shell, root_shell);
            }
        }
    }
    else
    {
        if (strcmp(argv[1], "-v") == 0)
        {
            printf("Eve 0.0.1\n");
            exit(1);
        }
        
        lexer_T* lexer = init_lexer(
            get_file_contents(argv[1])
        );
        parser_T* parser_file = init_parser(lexer);
        AST_T* root_file = parser_parse(parser_file, parser_file->scope);
        visitor_T* visitor_file = init_visitor();
        visitor_visit(visitor_file, root_file);
    }

    return 0;
}