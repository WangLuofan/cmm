#ifndef __AST_H__
#define __AST_H__

#include <stdint.h>

#include "type.h"

typedef enum NodeKind {
    NodeKind_NullExpr,
    NodeKind_Variable,
    NodeKind_Number,
    NodeKind_VarDecl,
    NodeKind_Function,
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

typedef struct ASTNodeVarDecl {
    struct ASTNode ast;
    struct Type *ty;
}ASTNodeDecl;

typedef struct ASTNodeFunction {
    struct ASTNode ast;

    const char *name;
    struct Type *ret_ty;
}ASTNodeFunction;

struct ASTNode *newast(NodeKind, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_list(struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_var(char *, struct ASTNode *);
struct ASTNode *newast_num(uint64_t);
struct ASTNode *newast_vardecl(struct Type *, struct ASTNode *);
struct ASTNode *newast_function(struct Type *, const char *, struct ASTNode *, struct ASTNode *);

#endif