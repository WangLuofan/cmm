#include "ast.h"

#include <string.h>
#include <stdlib.h>

struct ASTNode *newast_node(NodeKind kind, struct ASTNode *left, struct ASTNode *right) {
    struct ASTNode *ast = (struct ASTNode *)malloc(sizeof(struct ASTNode));
    
    ast->kind = kind;
    ast->left = left;
    ast->right = right;

    return ast;
}

struct ASTNode *newast_num(int val) {
    struct ASTNodeNum *num = (struct ASTNodeNum *)malloc(sizeof(struct ASTNodeNum));
    num->ast.kind = NodeKind_Number;
    num->value.ival = val;
    return num;
}

struct ASTNode *newast_var(char *name, struct Type *ty, struct ASTNode *val) {
    struct ASTNodeVar *var = (struct ASTNodeVar *)malloc(sizeof(struct ASTNodeVar));
    
    var->ast.kind = NodeKind_Variable;
    var->name = strdup(name);
    var->val = val;
    var->ty = copy_type(ty);

    return var;
}

struct ASTNode *newast_vardecl(struct Type *ty, struct ASTNode *var) {
    struct ASTNodeList *var_list = (struct ASTNodeList *)var;
    while (var_list && var_list->node) {
        ((struct ASTNodeVar *)var_list->node)->ty = copy_type(ty);
        var_list = var_list->next;
    }    
    return var;
}

struct ASTNode *newast_list(struct ASTNode *list, struct ASTNode *node) {
    struct ASTNodeList *next = (struct ASTNodeList *)malloc(sizeof(struct ASTNodeList));

    next->node = node;
    next->next = NULL;
    ++next->index;

    if (list == NULL) {
        return next;
    }

    struct ASTNodeList *cur = list, *prev = NULL;
    while (cur) {
        prev = cur;
        cur = cur->next;
    }

    prev->next = next;

    return list;
}

struct ASTNode *newast_function(struct Type *ret_ty, const char *name, struct ASTNode *params, struct ASTNode *body) {
    struct ASTNodeFunction *func = (struct ASTNodeFunction *)malloc(sizeof(struct ASTNodeFunction));

    func->ast.kind = NodeKind_Function;
    func->ret_ty = copy_type(ret_ty);
    func->name = strdup(name);
    func->ast.left = params;
    func->ast.right = body;

    return func;
}

struct ASTNode *newast_fncall(const char *name, struct ASTNode *args) {
    struct ASTNodeFnCall *call = (struct ASTNodeFnCall *)malloc(sizeof(struct ASTNodeFnCall));

    call->ast.kind = NodeKind_FnCall;
    call->name = strdup(name);
    call->ast.left = args;

    return call;
}

struct ASTNode *newast_compoundstmt(const char *fn, struct ASTNode *stmts) {

    struct ASTNodeCompoundStmt *stmt = (struct ASTNodeCompoundStmt *)malloc(sizeof(struct ASTNodeCompoundStmt));
    
    stmt->ast.kind = NodeKind_CompoundStmt;
    stmt->ast.left = stmts;

    if (fn) {
        stmt->fn = strdup(fn);
    }

    return stmt;
}