#include "ast.h"

#include <string.h>
#include <stdlib.h>

struct ASTNode *newast(NodeKind kind, struct ASTNode *left, struct ASTNode *right) {
    struct ASTNode *ast = (struct ASTNode *)malloc(sizeof(struct ASTNode));
    
    ast->kind = kind;
    ast->left = left;
    ast->right = right;

    return ast;
}

struct ASTNode *newast_num(uint64_t val) {
    struct ASTNodeNum *num = (struct ASTNodeNum *)malloc(sizeof(struct ASTNodeNum));
    num->ast.kind = NodeKind_Number;
    num->ival = val;
    return num;
}

struct ASTNode *newast_var(char *name, struct ASTNode *val) {
    struct ASTNodeVar *var = (struct ASTNodeVar *)malloc(sizeof(struct ASTNodeVar));
    
    var->ast.kind = NodeKind_Variable;
    var->name = strdup(name);
    var->val = val;

    return var;
}

struct ASTNode *newast_decl(struct Type *ty, struct ASTNode *var) {
    struct ASTNodeDecl *decl = (struct ASTNodeDecl *)malloc(sizeof(struct ASTNodeDecl));

    decl->ast.kind = NodeKind_Decl;
    decl->ty = copy_type(ty);
    decl->ast.left = var;

    return decl;
}

struct ASTNode *newast_list(struct ASTNode *node, struct ASTNode *next) {
    struct ASTNodeList *list = (struct ASTNodeList *)malloc(sizeof(struct ASTNodeList));

    list->node = node;
    list->next = next;

    return list;
}