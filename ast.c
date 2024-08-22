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

struct ASTNode *newast_num(int val) {
    struct ASTNodeNum *num = (struct ASTNodeNum *)malloc(sizeof(struct ASTNodeNum));
    num->value = val;
    return num;
}

struct ASTNode *newast_var(char *name) {
    struct ASTNodeVar *var = (struct ASTNodeVar *)malloc(sizeof(struct ASTNodeVar));
    var->ast.kind = NodeKind_Variable;
    var->name = strdup(name);
    return var;
}

struct ASTNode *newast_decl(struct Type *ty, struct ASTNode *var, struct ASTNode *val) {
    struct ASTNodeDecl *decl = (struct ASTNodeDecl *)malloc(sizeof(struct ASTNodeDecl));

    decl->ast.kind = NodeKind_Decl;
    decl->ty = copy_type(ty);
    decl->ast.left = var;
    decl->ast.right = val;

    return decl;
}