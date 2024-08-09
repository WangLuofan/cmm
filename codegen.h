#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <stdio.h>

#include "ast.h"

void emit_code(struct ASTNode *, FILE *);

#endif