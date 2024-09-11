#ifndef __UTILS_H__
#define __UTILS_H__

struct ASTNode;
typedef enum CompInstKind CompInstKind;
typedef enum CompKind CompKind;

int align_to(int, int);
int is_const_expr(struct ASTNode *);

CompInstKind comp_type(CompKind);

#endif