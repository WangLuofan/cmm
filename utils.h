#ifndef __UTILS_H__
#define __UTILS_H__

struct ASTNode;

int align_to(int, int);
int is_const_expr(struct ASTNode *);

#endif