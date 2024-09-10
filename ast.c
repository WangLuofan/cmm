#include "ast.h"
#include "symbol.h"

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
    num->ty = copy_type(ty_int);
    return num;
}

struct ASTNode *newast_var(char *name, struct Type *ty, struct ASTNode *expr) {
    struct ASTNodeVar *var = (struct ASTNodeVar *)malloc(sizeof(struct ASTNodeVar));
    
    var->ast.kind = NodeKind_Variable;
    var->ast.right = expr;
    var->name = strdup(name);

    var->sym = new_sym(name);    
    var->sym->ty = copy_type(ty);

    return var;
}

struct ASTNode *newast_vardecl(struct Type *ty, struct ASTNode *ast) {
    struct ASTNodeVarDecl *decl = (struct ASTNodeVarDecl *)malloc(sizeof(struct ASTNodeVarDecl));

    decl->ast.kind = NodeKind_VarDecl;
    decl->ty = copy_type(ty);
    decl->var = ast;

    struct ASTNodeList *list = ((struct ASTNodeList *)ast)->next;
    while (list && list->node) {
        struct ASTNodeVar *var = (struct ASTNodeVar *)list->node;
        var->sym->ty = copy_type(ty);

        list = list->next;
    }

    return decl;
}

struct ASTNode *newast_list(struct ASTNodeList *list, struct ASTNode *node) {
    if (list == NULL) {
        list = (struct ASTNodeList *)malloc(sizeof(struct ASTNodeList));
        list->node = NULL;
        list->next = list;
        list->prev = list;
    }

    struct ASTNodeList *next = (struct ASTNodeList *)malloc(sizeof(struct ASTNodeList));
    next->node = node;

    list->prev->next = next;
    next->prev = list->prev;
    next->next = list;
    list->prev = next;

    return list;
}

struct ASTNode *newast_function(struct Type *ret_ty, const char *name, struct ASTNode *params, struct ASTNode *body) {
    struct ASTNodeFunction *func = (struct ASTNodeFunction *)malloc(sizeof(struct ASTNodeFunction));

    func->ast.kind = NodeKind_Function;
    func->name = strdup(name);
    func->ast.left = params;
    func->ast.right = body;
    
    func->sym = new_sym(name);
    func->sym->ty = copy_type(ret_ty);

    return func;
}

struct ASTNode *newast_fncall(const char *name, struct ASTNode *args) {
    struct ASTNodeFnCall *call = (struct ASTNodeFnCall *)malloc(sizeof(struct ASTNodeFnCall));

    call->ast.kind = NodeKind_FnCall;
    call->name = strdup(name);
    call->ast.left = args;

    return call;
}

struct ASTNode *newast_compoundstmt(struct ASTNode *stmts) {

    struct ASTNodeCompoundStmt *stmt = (struct ASTNodeCompoundStmt *)malloc(sizeof(struct ASTNodeCompoundStmt));
    
    stmt->ast.kind = NodeKind_CompoundStmt;
    stmt->ast.left = stmts;

    return stmt;
}

struct ASTNode *newast_arith(ArithKind kind, struct ASTNode *left, struct ASTNode *right) {
    struct ASTNodeArith *arith = (struct ASTNodeArith *)malloc(sizeof(struct ASTNodeArith));

    arith->kind = kind;
    arith->ast.kind = NodeKind_Arith;
    arith->ast.left = left;
    arith->ast.right = right;

    return arith;
}

struct ASTNode *merge_list(struct ASTNodeList *list1, struct ASTNodeList *list2) {

    if (list2 == NULL) {
        return list1;
    }

    if (list1 == NULL) {
        return list2;
    }

    list1->prev->next = list2->next;
    list2->next->prev = list1->prev;
    list1->prev = list2->prev;
    list2->prev->next = list1;

    return list1;
}