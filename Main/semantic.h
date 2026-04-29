#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

void semantic_check(ASTNode *root);
int semantic_has_errors(void);

#endif
