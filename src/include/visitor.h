#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"

typedef struct VISITOR_STRUCT
{
    
} visitor_T;

visitor_T* init_visitor();

AST_T* visitor_visit(visitor_T* visitor, AST_T* node); // take in the root node and check the types and visit

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node); // visit the variabe definition

AST_T* visitor_visit_function_definition(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node); // visit the variable and look up the memory

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node); // visit function call

AST_T* visitor_visit_list_definition(visitor_T* visitor, AST_T* node); // visit list definition

AST_T* visitor_visit_list(visitor_T* visitor, AST_T* node); // visit list and look up the memory

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node); // loop the compound children

#endif