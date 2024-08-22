#ifndef __AST_H__
#define __AST_H__

#include <stdint.h>

#include "type.h"

typedef enum NodeKind {
    NodeKind_NullExpr,
    NodeKind_Variable,
    NodeKind_Number,
    NodeKind_Decl,
}NodeKind;

typedef struct ASTNode {
    NodeKind kind;

    struct ASTNode *left;
    struct ASTNode *right;
}ASTNode;

typedef struct ASTNodeVar {
    struct ASTNode ast;
    const char *name;
}ASTNodeVar;

typedef struct ASTNodeNum {
    struct ASTNode ast;
    uint64_t value;
}ASTNodeNum;

typedef struct ASTNodeDecl {
    struct ASTNode ast;
    struct Type *ty;
}ASTNodeDecl;

struct ASTNode *newast(NodeKind, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_var(char *);
struct ASTNode *newast_num(int);
struct ASTNode *newast_decl(struct Type *, struct ASTNode *, struct ASTNode *);

#endif