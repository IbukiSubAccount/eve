#include "include/scope.h"
#include <string.h>
#include <stdio.h>

scope_T* init_scope()
{
    scope_T* scope = calloc(1, sizeof(struct SCOPE_STRUCT));
    scope->function_definitions = (void*) 0;
    scope->function_definitions_size = 0;

    scope->variable_definitions = (void*) 0;
    scope->variable_definitions_size = 0;

    scope->list_definitions = (void*) 0;
    scope->list_definitions_size = 0;

    return scope;
}

AST_T* scope_add_function_definition(scope_T* scope, AST_T* fdef)
{
    scope->function_definitions_size += 1;

    if (scope->function_definitions == (void*) 0)
    {
        scope->function_definitions = calloc(1, sizeof(struct AST_STRUCT*));
    }
    else
    {
        scope->function_definitions = 
            realloc(
                scope->function_definitions,
                scope->function_definitions_size * sizeof(struct AST_STRUCT**)
            );
    }

    scope->function_definitions[scope->function_definitions_size-1] = fdef;

    return fdef;
}

AST_T* scope_get_function_definition(scope_T* scope, const char* fname)
{
    for (int i = 0; i < scope->function_definitions_size; i++)
    {
        AST_T* fdef = scope->function_definitions[i];
        if (strcmp(fdef->function_definition_name, fname) == 0)
        {
            return fdef;
        }
    }

    return (void*)0;
}

AST_T* scope_add_variable_definition(scope_T* scope, AST_T* vdef)
{
    scope->variable_definitions_size += 1; // increment the size

    if (scope->variable_definitions == (void*) 0) // if list is null
    {
        scope->variable_definitions = calloc(1, sizeof(struct AST_STRUCT*)); // allocating memory
    }
    else // if the list already exists
    {
        scope->variable_definitions =
            realloc( // reallocating the memory
                scope->variable_definitions,
                scope->variable_definitions_size * sizeof(struct AST_STRUCT*)
            ); 
    }

    scope->variable_definitions[scope->variable_definitions_size-1] = vdef; // put the node in the end of the list

    return vdef;
}

AST_T* scope_get_variable_definition(scope_T* scope, const char* name)
{
    for (int i = 0; i < scope->variable_definitions_size; i++)
    {
        AST_T* vdef = scope->variable_definitions[i];
        if (strcmp(vdef->variable_definition_variable_name, name) == 0)
        {
            return vdef;
        }
    }

    return (void*)0;
}

AST_T* scope_add_list_definition(scope_T* scope, AST_T* ldef)
{
    scope->list_definitions_size += 1;

    if (scope->list_definitions == (void*) 0)
    {
        scope->list_definitions = calloc(1, sizeof(struct AST_STRUCT*));
    }
    else
    {
        scope->list_definitions = 
            realloc(
                scope->list_definitions,
                scope->list_definitions_size * sizeof(struct AST_STRUCT**)
            );
    }

    scope->list_definitions[scope->list_definitions_size-1] = ldef;

    return ldef;
}


AST_T* scope_get_list_definition(scope_T* scope, const char* lname)
{
    for (int i = 0; i < scope->list_definitions_size; i++)
    {
        AST_T* ldef = scope->list_definitions[i];
        if (strcmp(ldef->list_definition_name, lname) == 0)
        {
            return ldef;
        }
    }

    return (void*)0;
}