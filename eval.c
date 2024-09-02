#include "ast.h"
#include "eval.h"
#include "utils.h"

int eval(struct ASTNode *expr) {
    if (!is_const_expr(expr)) {
        return -1;
    }
    
    switch (expr->kind) {
        case NodeKind_Number: {
            struct ASTNodeNum *num = (struct ASTNodeNum *)expr;
            return num->value.ival;
        }
            break;
        case NodeKind_Variable: {
            struct ASTNodeVar *var = (struct ASTNodeVar *)expr;
            return eval(var->ast.right);
        }
            break;
    }

    return -1;
}