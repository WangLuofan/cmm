#include "utils.h"
#include "ast.h"

#include "instruction.h"

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

CompInstKind comp_type(CompKind kind) {
    switch (kind) {
        case CompKind_LessThan:
            return CompInstKindLessThan;
        case CompKind_LessEqual:
            return CompInstKindLessEqual;
        case CompKind_Equal:
            return CompInstKindEqual;
        case CompKind_NotEqual:
            return CompInstKindNotEqual;
        case CompKind_GreaterThan:
            return CompInstKindGreaterThan;
        case CompKind_GreaterEqual:
            return CompInstKindGreaterEqual;
    }

    return CompInstKindNone;
}