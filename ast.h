#ifndef __AST_H__
#define __AST_H__

typedef enum NodeKind {
    NodeKind_Declaration
}NodeKind;

typedef struct ASTNode {
    NodeKind kind;
    struct ASTNode *left;
    struct ASTNode *right;
}ASTNode;

struct ASTNode *new_declaration(void);

#endif