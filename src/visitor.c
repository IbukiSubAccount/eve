#include "include/visitor.h"
#include "include/AST.h"
#include <stdio.h>

visitor_T* init_visitor()
{
    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
    visitor->variable_definitions = calloc(1, sizeof(struct AST_STRUCT*));
    visitor->variable_definitions_size = 0;

    return visitor;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node)
{
    switch (node->type)
    {
        case AST_VARIABLE_DEFINITION: return visitor_visit_variable_definition(visitor, node); break;
        case AST_VARIABLE: return visitor_visit_variable(visitor, node); break;
        case AST_FUNCTION_CALL: return visitor_visit_function_call(visitor, node); break;
        case AST_STRING: return visitor_visit_string(visitor, node); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node); break;
        case AST_NOOP: return node; break;
    }

    printf("Uncaught statement of type '%d'\n", node->type);
    exit(1);

    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node)
{
    printf("visitor visit variable definition\n");
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node)
{

}

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node)
{

}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{

}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < node->compound_size; i++)
    {
        visitor_visit(node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}