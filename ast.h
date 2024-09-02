#ifndef __AST_H__
#define __AST_H__

#include <stdint.h>

#include "type.h"

typedef enum NodeKind {
    NodeKind_NullExpr,
    NodeKind_Variable,
    NodeKind_Number,
    NodeKind_Function,
    NodeKind_FnCall,
    NodeKind_CompoundStmt,
    NodeKind_CommaExpr,
    NodeKind_ExprStmt,
    NodeKind_VarDecl
}NodeKind;

typedef struct ASTNode {
    NodeKind kind;

    struct ASTNode *left;
    struct ASTNode *right;
}ASTNode;

typedef struct ASTNodeVar {
    struct ASTNode ast;

    char *name;
    struct Type *ty;

    int offset;
}ASTNodeVar;

typedef struct ASTNodeVarDecl {
    struct ASTNode ast;

    struct Type *ty;
    struct ASTNode *var;
}ASTNodeVarDecl;

typedef struct ASTNodeList {
    struct ASTNode ast;

    struct ASTNode *node;

    struct ASTNodeList *prev;
    struct ASTNodeList *next;
}ASTNodeList;

typedef struct ASTNodeNum {
    struct ASTNode ast;
    union value {
        int ival;
    }value;
    
}ASTNodeNum;

typedef struct ASTNodeFunction {
    struct ASTNode ast;

    char *name;
    int stack_size;
    struct Type *ret_ty;
    struct ASTNode *locals;
}ASTNodeFunction;

typedef struct ASTNodeFnCall {
    struct ASTNode ast;
    char *name;
}ASTNodeFnCall;

typedef struct ASTNodeCompoundStmt {
    struct ASTNode ast;
    struct ASTNode *fn;
}ASTNodeCompoundStmt;

struct ASTNode *newast_node(NodeKind, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_list(struct ASTNodeList *, struct ASTNode *);
struct ASTNode *newast_var(char *, struct Type *, struct ASTNode *);
struct ASTNode *newast_num(int);
struct ASTNode *newast_compoundstmt(struct ASTNode *);
struct ASTNode *newast_vardecl(struct Type *, struct ASTNode *);
struct ASTNode *newast_function(struct Type *, const char *, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_fncall(const char *, struct ASTNode *);
struct ASTNode *merge_list(struct ASTNodeList *, struct ASTNodeList *);

#endif