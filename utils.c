#include "utils.h"
#include "ast.h"

int align_to(int bytes, int align) {
    return (bytes + align - 1) / align * align;
}

int is_const_expr(struct ASTNode *node) {
    int is_const = 0;
    switch (node->kind) {
        case NodeKind_Number: {
            is_const = 1;
        }
            break;
        case NodeKind_Variable: {
            struct ASTNodeVar *var = (struct ASTNodeVar *)node;
            return is_const_expr(var->ast.right);
        }
            break;
        default: {
            is_const = 0;
        }
            break;
    }
    return is_const;
}