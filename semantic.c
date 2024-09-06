#include "ast.h"
#include "scope.h"
#include "symbol.h"
#include "semantic.h"

#include <stdio.h>

struct ASTNodeFunction *current_func = NULL;
struct Symbol *locals = NULL;

void SEM_expr(struct ASTNode *expr) {
    switch (expr->kind) {
        case NodeKind_Variable: {
            struct ASTNodeVar *var = (struct ASTNodeVar *)expr;
            struct Symbol *sym = (struct Symbol *)find_scope(var->name);

            if (sym) {
                var->sym = sym;
            }
        }
            break;
        case NodeKind_Add: {
            SEM_expr(expr->left);
            SEM_expr(expr->right);
        }
            break;
    }
}

void SEM_stmt(struct ASTNode *stmt) {
    switch (stmt->kind) {
        case NodeKind_CompoundStmt: {
            struct ASTNodeCompoundStmt *compStmt = (struct ASTNodeCompoundStmt *)stmt;
            push_scope();

            if (compStmt->fn != NULL) {
                struct ASTNodeFunction *fn = (struct ASTNodeFunction *)compStmt->fn;
                if (fn->ast.left) {
                    struct ASTNodeList *params = ((struct ASTNodeList *)((struct ASTNodeFunction *)compStmt->fn)->ast.left)->next;
                    while (params && params->node) {
                        struct Symbol *sym = ((struct ASTNodeVar *)params->node)->sym;
                        put_scope(sym->sym, sym);
                        params = params->next;
                    }
                }
            }

            if (compStmt->ast.left == NULL) {
                return ;
            }

            struct ASTNodeList *stmts = ((struct ASTNodeList *)compStmt->ast.left)->next;

            while (stmts && stmts->node) {
                SEM_stmt(stmts->node);
                stmts = stmts->next;
            }

            pop_scope();
        }
            break;
        case NodeKind_VarDecl: {
            struct ASTNodeVarDecl *decl = (struct ASTNodeVarDecl *)stmt;
            struct ASTNodeList *varlist = ((struct ASTNodeList *)decl->var)->next;

            while (varlist && varlist->node) {
                struct ASTNodeVar *var = (struct ASTNodeVar *)varlist->node;
                var->sym = locals;
                locals = var->sym;

                varlist = varlist->next;
            }
        }
            break;
        case NodeKind_ExprStmt: {
            SEM_expr(stmt->left);
        }
            break;
        case NodeKind_Return: {
            SEM_expr(stmt->left);
        }
            break;
    }
}

int SEM_prog(struct ASTNode *prog) {
    struct ASTNodeList *list = ((struct ASTNodeList *)prog)->next;
    while (list && list->node) { 
        switch (list->node->kind) {
            case NodeKind_Function: {
                struct ASTNodeFunction *func = (struct ASTNodeFunction *)list->node;

                struct ASTNodeCompoundStmt *body = (struct ASTNodeCompoundStmt *)func->ast.right;

                current_func = func;

                body->fn = func;
                SEM_stmt(body);

                func->sym->locals = locals;

                locals = NULL;
                current_func = NULL;
            }
                break;
            case NodeKind_VarDecl: {
                struct ASTNodeVarDecl *decl = (struct ASTNodeVarDecl *)list->node;
                struct ASTNodeList *list = ((struct ASTNodeList *)decl->var)->next;
                while (list && list->node) {
                    struct ASTNodeVar *var = (struct ASTNodeVar *)list->node;
                    put_scope(var->name, var->sym);

                    list = list->next;
                }
            }
                break;
        }

        list = list->next;
    }
    return 0;
}