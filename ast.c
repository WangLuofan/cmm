#include <stdlib.h>

#include "ast.h"

struct ASTNode *new_declaration(void) {
    struct ASTNode *node = (struct ASTNode *)malloc(sizeof(struct ASTNode));
    node->kind = NodeKind_Declaration;
    return node;
}