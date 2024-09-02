#include "ast.h"
#include "semantic.h"

#include <stdio.h>

struct ASTNodeFunction *current_func = NULL;
struct ASTNode *locals = NULL;

int SEM_stmt(struct ASTNode *stmt) {
    switch (stmt->kind) {
        case NodeKind_CompoundStmt: {
            struct ASTNodeCompoundStmt *compStmt = (struct ASTNodeCompoundStmt *)stmt;
            struct ASTNodeList *stmts = ((struct ASTNodeList *)compStmt->ast.left)->next;

            while (stmts && stmts->node) {
                SEM_stmt(stmts->node);
                stmts = stmts->next;
            }
        }
            break;
        case NodeKind_VarDecl: {
            struct ASTNodeVarDecl *decl = (struct ASTNodeVarDecl *)stmt;
            struct ASTNodeList *varlist = (struct ASTNodeList *)decl->var;
            locals = merge_list(locals, varlist);
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

                struct ASTNodeList *params = (struct ASTNodeList *)func->ast.left;
                struct ASTNodeCompoundStmt *body = (struct ASTNodeCompoundStmt *)func->ast.right;

                current_func = func;
                SEM_stmt(body);

                func->locals = locals;

                locals = NULL;
                current_func = NULL;
            }
                break;
        }

        list = list->next;
    }
    return 0;
}