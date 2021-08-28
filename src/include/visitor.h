#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"

/* for this program
v name = "Ibuki Yoshida";
eve(name);
*/

AST_T* visitor_visit(AST_T* node); // 1. take in the root node and check the types and visit

AST_T* visitor_visit_variable_definition(AST_T* node); // 3. visit the variabe definition

AST_T* visitor_visit_variable(AST_T* node); // 5. visit the variable and look up the memory

AST_T* visitor_visit_function_call(AST_T* node); // 4. visit function call

AST_T* visitor_visit_string(AST_T* node);

AST_T* visitor_visit_compound(AST_T* node); // 2. loop the compound children

#endif