#include "include/visitor.h"
#include "include/AST.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static AST_T* builtin_function_print(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_STRING: printf("%s", visited_ast->string_value); break;
            case AST_INT: printf("%lld", visited_ast->int_value); break;
            default: printf("%p\n", visited_ast); break;
        }
    }
    printf("\n");

    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_sleep(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);
        
        switch (visited_ast->type)
        {
            case AST_INT: sleep(visited_ast->int_value); break;
            default: printf("Error: Sleep function expecting integer\n"); exit(1);
        }
    }

    return init_ast(AST_NOOP);
}

static AST_T* builtin_function_browser(visitor_T* visitor, AST_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T* visited_ast = visitor_visit(visitor, args[i]);
        #if __APPLE__
            char s[200] = "open ";
        #elif _WIN32
            char s[200] = "start ";
        #elif __LINUX__
            char s[200] = "xdg-open ";
        #else
            printf("Error: Browser function Unexpected OS");
            exit(1);
        #endif

        char *p = visited_ast->string_value;
        strcat(s, p);
        printf("%s\n", s);
        switch (visited_ast->type)
        {
            case AST_STRING: system(s); break;
            default: printf("Error Browser function: expecting string\n"); exit(1);
        }
    }

    return init_ast(AST_NOOP);
}

visitor_T* init_visitor()
{
    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));

    return visitor;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node)
{
    switch (node->type)
    {
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor, node); break;
        case AST_FUNCTION_DEFINITION: return visitor_visit_function_definition(visitor, node); break;
        case AST_VARIABLE: return visitor_visit_variable(visitor, node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor, node); break;
        case AST_STRING: return visitor_visit_string(visitor, node); break;
        case AST_INT: return visitor_visit_int(visitor, node); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node); break;
        case AST_NOOP: return node; break;
    }

    printf("\x1b[31m");
    printf("Error: Uncaught statement of type '%d'\n", node->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    scope_add_variable_definition(
        node->scope,
        node
    );

    return node;
}

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node)
{
    // printf("We found the function definition! %s\n", node->function_definition_name);

    scope_add_function_definition(
        node->scope,
        node
    );
    return node;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node)
{
    AST_T* vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name
    );

    if (vdef != (void*) 0)
        return visitor_visit(visitor, vdef->variable_definition_value);
    
    printf("Error: Undifined variable '%s'\n", node->variable_name);
    exit(1);
}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node)
{
    if (strcmp(node->function_call_name, "eve") == 0)
    {
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }
    if (strcmp(node->function_call_name, "sleep") == 0)
    {
        return builtin_function_sleep(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }
    if (strcmp(node->function_call_name, "browser") == 0)
    {
        return builtin_function_browser(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }
    if (strcmp(node->function_call_name, "exit") == 0)
    {
        exit(1);
    }

    AST_T* fdef = scope_get_function_definition(
        node->scope,
        node->function_call_name
    );

    if (fdef == (void*)0)
    {
        printf("Error: Undifiend method '%s'\n", node->function_call_name);
        exit(1);
    }

    for (int i = 0; i < (int) node->function_call_arguments_size; i++)
    {
        // grab the variable from the function definition arguments.
        AST_T* ast_var = (AST_T*) fdef->function_definition_args[i];

        // grab the value from the function call arguments.
        AST_T* ast_value = (AST_T*) node->function_call_arguments[i];

        //  create a new variable definition with the value of the argument.
        // in the function call
        AST_T* ast_vardef = init_ast(AST_VARIABLE_DEFINITION);

        // copy the name from the function definition argument into the new
        // variable definition.
        ast_vardef->variable_definition_value = ast_value;

        // push our variable definition into the function body scope.
        ast_vardef->variable_definition_variable_name = calloc(strlen(ast_var->variable_name) + 1, sizeof(char));
        strcpy(ast_vardef->variable_definition_variable_name, ast_var->variable_name);

        scope_add_variable_definition(fdef->function_definition_body->scope, ast_vardef);
    }
    
    // printf("Found it!\n");
    return visitor_visit(visitor, fdef->function_definition_body);

}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node)
{
    return node;
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}