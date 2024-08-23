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
    struct ASTNode *val;
}ASTNodeVar;

typedef struct ASTNodeList {
    struct ASTNode ast;

    struct ASTNode *node;
    struct ASTNode *next;
}ASTNodeList;

typedef struct ASTNodeNum {
    struct ASTNode ast;
    uint64_t ival;
}ASTNodeNum;

typedef struct ASTNodeDecl {
    struct ASTNode ast;
    struct Type *ty;
}ASTNodeDecl;

struct ASTNode *newast(NodeKind, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_list(struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_var(char *, struct ASTNode *);
struct ASTNode *newast_num(uint64_t);
struct ASTNode *newast_decl(struct Type *, struct ASTNode *);

#endif