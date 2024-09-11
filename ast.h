#ifndef __AST_H__
#define __AST_H__

#include <stdint.h>

#include "type.h"

typedef struct Symbol;
typedef enum NodeKind {
    NodeKind_NullExpr,
    NodeKind_Variable,
    NodeKind_Number,
    NodeKind_Function,
    NodeKind_FnCall,
    NodeKind_CompoundStmt,
    NodeKind_CommaExpr,
    NodeKind_ExprStmt,
    NodeKind_VarDecl,
    NodeKind_Return,
    NodeKind_ArithExpr,
    NodeKind_CompExpr,
    NodeKind_IfStmt,
}NodeKind;

typedef enum ArithKind {
    ArithKind_Add,
    ArithKind_Sub,
    ArithKind_Mul,
    ArithKind_Div,
}ArithKind;

typedef enum CompKind {
    CompKind_LessThan,
    CompKind_LessEqual,
    CompKind_GreaterThan,
    CompKind_GreaterEqual,
    CompKind_Equal,
    CompKind_NotEqual
}CompKind;

typedef struct ASTNode {
    NodeKind kind;

    struct ASTNode *left;
    struct ASTNode *right;
}ASTNode;

typedef struct ASTNodeVar {
    struct ASTNode ast;

    char *name;
    struct Symbol *sym;

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
    struct Type *ty;
    union value {
        int ival;
    }value;
    
}ASTNodeNum;

typedef struct ASTNodeFunction {
    struct ASTNode ast;

    char *name;
    struct Symbol *sym;
}ASTNodeFunction;

typedef struct ASTNodeFnCall {
    struct ASTNode ast;
    char *name;
}ASTNodeFnCall;

typedef struct ASTNodeCompoundStmt {
    struct ASTNode ast;
    struct ASTNode *fn;
}ASTNodeCompoundStmt;

typedef struct ASTNodeIfStmt {
    struct ASTNode ast;
    char *label;
    
    struct ASTNode *cond;
    struct ASTNode *then;
    struct ASTNode *els;
}ASTNodeIfStmt;

typedef struct ASTNodeArithExpr {
    struct ASTNode ast;
    ArithKind kind;
}ASTNodeArithExpr;

typedef struct ASTNodeCompExpr {
    struct ASTNode ast;
    CompKind kind;
}ASTNodeCompExpr;

struct ASTNode *newast_node(NodeKind, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_arith_expr(ArithKind, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_comp_expr(CompKind, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_list(struct ASTNodeList *, struct ASTNode *);
struct ASTNode *newast_var(char *, struct Type *, struct ASTNode *);
struct ASTNode *newast_num(int);
struct ASTNode *newast_compoundstmt(struct ASTNode *);
struct ASTNode *newast_ifstmt(struct ASTNode *, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_vardecl(struct Type *, struct ASTNode *);
struct ASTNode *newast_function(struct Type *, const char *, struct ASTNode *, struct ASTNode *);
struct ASTNode *newast_fncall(const char *, struct ASTNode *);
struct ASTNode *merge_list(struct ASTNodeList *, struct ASTNodeList *);

#endif