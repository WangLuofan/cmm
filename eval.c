#include "ast.h"
#include "eval.h"
#include "utils.h"

int eval(struct ASTNode *expr) {
    if (!is_const_expr(expr)) {
        return -1;
    }
    
    switch (expr->kind) {
        case NodeKind_Number:
            return ((struct ASTNodeNum *)expr)->val.ival;
        case NodeKind_Variable:
            return eval(expr->right);
        case NodeKind_ArithExpr: {
            struct ASTNodeArithExpr *arith = (struct ASTNodeArithExpr *)expr;
            switch (arith->kind) {
                case ArithKind_Add:
                    return eval(arith->ast.left) + eval(arith->ast.right);
                case ArithKind_Sub:
                    return eval(arith->ast.left) - eval(arith->ast.right);
                case ArithKind_Mul:
                    return eval(arith->ast.left) * eval(arith->ast.right);
                case ArithKind_Div:
                    return eval(arith->ast.left) / eval(arith->ast.right);
            }
        }
            break;
        case NodeKind_CompExpr: {
            struct ASTNodeCompExpr *comp = (struct ASTNodeCompExpr *)expr;
            switch (comp->kind) {
                case CompKind_LessThan:
                    return eval(comp->ast.left) < eval(comp->ast.right);
                case CompKind_LessEqual:
                    return eval(comp->ast.left) <= eval(comp->ast.right);
                case CompKind_Equal:
                    return eval(comp->ast.left) == eval(comp->ast.right);
                case CompKind_NotEqual:
                    return eval(comp->ast.left) != eval(comp->ast.right);
                case CompKind_GreaterThan:
                    return eval(comp->ast.left) > eval(comp->ast.right);
                case CompKind_GreaterEqual:
                    return eval(comp->ast.left) >= eval(comp->ast.right);
            }
        }
            break;
    }

    return -1;
}