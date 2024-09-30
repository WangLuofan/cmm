#include "ast.h"
#include "eval.h"
#include "utils.h"
#include "frame.h"
#include "symbol.h"
#include "context.h"
#include "codegen.h"
#include "instruction.h"

#include <string.h>
#include <stdlib.h>

static int func_index = 0;
static int inner_index = 0;

const char *unique_label() {
    char *label = (char *)malloc(sizeof(char) * 20);
    sprintf(label, ".L.%d.%d", func_index, inner_index++);

    return label;
}

void prologue(FILE *fp, int stack_size) {
    fprintf(fp, "\tendbr64\n");
    fprintf(fp, "\t%s\t\t%s\n", push(), bp());
    fprintf(fp, "\t%s\t\t%s, %s\n", mov(8), sp(), bp());

    if (stack_size) {
        fprintf(fp, "\t%s\t\t$%d, %s\n", isub(8), align_to(stack_size, 16), sp());
    }
}

void epilogue(FILE *fp) {
    fprintf(fp, ".L.return.%d:\n", func_index);
    fprintf(fp, "\t%s\t\t%s, %s\n", mov(8), bp(), sp());
    fprintf(fp, "\t%s\t\t%s\n", pop(), bp());
    fprintf(fp, "\t%s\n", ret());
}

const char *(*inst(int arith))(int) {
    switch (arith) {
        case ArithKind_Add:
            return iadd;
        case ArithKind_Sub:
            return isub;
        case ArithKind_Mul:
            return imul;
        case ArithKind_Div:
            return idiv;
    }

    return NULL;
}

void comp_zero(FILE *fp, struct ASTNode *expr) {
    switch (expr->kind) {
        case NodeKind_CompExpr: {
            return ;
        }
            break;
        default:
            fprintf(fp, "\t%s\t\t$%d, %s\n", cmp(4), 0, allocated_register(4));
            break;
    }
    
    unallocate_register();
    return ;
}

void emit_expr(FILE *fp, struct ASTNode *expr) {
    if (!expr) {
        return ;
    }

    switch (expr->kind) {
        case NodeKind_Variable: {
            struct ASTNodeVar *var = (struct ASTNodeVar *)expr;
            fprintf(fp, "\t%s\t\t%d(%s), %s\n", mov(var->sym->ty->size), var->sym->offset, bp(), allocate_register(var->sym->ty->size));
        }
            break;
        case NodeKind_FnCall: {
            struct ASTNodeList *args = ((struct ASTNodeList *)(((struct ASTNodeFnCall *)expr)->ast.left))->next;

            int gp = 0;
            while (args && args->node && gp < GP_MAX) {
                if (is_const_expr(args->node)) {
                    int res = eval(args->node);
                    fprintf(fp, "\t%s\t\t$%d, %s\n", mov(sizeof(res)), res, generic(gp, sizeof(res)));
                } else {
                    eval(args->node);
                    fprintf(fp, "\t%s\t\t%s, %s\n", mov(sizeof(int)), ax(sizeof(int)), generic(gp, sizeof(int)));
                }

                ++gp;
                args = args->next;
            }

            int depth = 0;
            if (args && args->node) {
                struct ASTNodeList *prev = ((struct ASTNodeList *)(((struct ASTNodeFnCall *)expr)->ast.left))->prev;
                while (prev && prev->node && prev != args->prev) {
                    ++depth;
                    if (is_const_expr(prev->node)) {
                        int res = eval(prev->node);
                        fprintf(fp, "\t%s\t\t$%d\n", push(), res);
                    } else {
                        eval(prev->node);
                        fprintf(fp, "\t%s\t\t%s\n", push(), ax(8));
                    }

                    prev = prev->prev;
                }
            }

            depth = align_to(depth << 3, 16) ?: 16;

            #ifdef __APPLE__
                fprintf(fp, "\t%s\t\t_%s\n", call(), ((struct ASTNodeFnCall *)expr)->name);
            #else
                fprintf(fp, "\t%s\t\t%s\n", call(), ((struct ASTNodeFnCall *)expr)->name);
            #endif
            fprintf(fp, "\t%s\t\t$%d, %s\n", iadd(8), depth, sp());
        }
            break;
        case NodeKind_Number: {
            struct ASTNodeNum *num = (struct ASTNodeNum *)expr;
            fprintf(fp, "\t%s\t\t$%d, %s\n", mov(num->ty->size), num->val.ival, ax(num->ty->size));
        }
            break;
        case NodeKind_ArithExpr: {
            struct ASTNodeArithExpr *arith = (struct ASTNodeArithExpr *)expr;

            const char *(*fn)(int) = inst(arith->kind);
            switch (expr->left->kind) {
                case NodeKind_Variable: {
                    struct ASTNodeVar *var = (struct ASTNodeVar *)expr->left;
                    fprintf(fp, "\t%s\t\t%d(%s), %s\n", mov(var->sym->ty->size), var->sym->offset, bp(), allocate_register(var->sym->ty->size));
                }
                    break;
                case NodeKind_Number: {
                    struct ASTNodeNum *num = (struct ASTNodeNum *)expr->left;
                    fprintf(fp, "\t%s\t\t$%d, %s\n", mov(num->ty->size), num->val.ival, allocate_register(num->ty->size));
                }
                    break;
                default: {
                    emit_expr(fp, expr->left);
                }
                    break;
            }

            switch (expr->right->kind) {
                case NodeKind_Variable: {
                    struct ASTNodeVar *var = (struct ASTNodeVar *)expr->right;
                    switch (arith->kind) {
                        case ArithKind_Div: {
                            if (strcmp(allocated_register(var->sym->ty->size), ax(var->sym->ty->size))) {
                                fprintf(fp, "\t%s\t\t%s\n", push(), ax(8));
                                fprintf(fp, "\t%s\t\t%s, %s\n", mov(var->sym->ty->size), allocated_register(var->sym->ty->size), ax(var->sym->ty->size));
                            }

                            fprintf(fp, "\t%s\n", clt());
                            fprintf(fp, "\t%s\t\t%d(%s)\n", fn(var->sym->ty->size), var->sym->offset, bp());

                            if (strcmp(allocated_register(var->sym->ty->size), ax(var->sym->ty->size))) {
                                fprintf(fp, "\t%s\t\t%s, %s\n", mov(var->sym->ty->size), ax(var->sym->ty->size), allocated_register(var->sym->ty->size));
                                fprintf(fp, "\t%s\t\t%s\n", pop(), ax(8));
                            }
                        }
                            break;
                        default: {
                            fprintf(fp, "\t%s\t\t%d(%s), %s\n", fn(var->sym->ty->size), var->sym->offset, bp(), allocated_register(var->sym->ty->size));
                        }
                            break;
                    }
                }
                    break;
                case NodeKind_Number: {
                    struct ASTNodeNum *num = (struct ASTNodeNum *)expr->right;
                    switch (arith->kind) {
                        case ArithKind_Div: {
                            if (strcmp(allocated_register(num->ty->size), ax(num->ty->size))) {
                                fprintf(fp, "\t%s\t\t%s\n", push(), ax(8));
                                fprintf(fp, "\t%s\t\t%s, %s\n", mov(num->ty->size), allocated_register(num->ty->size), ax(num->ty->size));
                            }

                            fprintf(fp, "\t%s\n", clt());
                            fprintf(fp, "\t%s\t\t$%d\n", fn(num->ty->size), num->val.ival, allocated_register(num->ty->size));

                            if (strcmp(allocated_register(num->ty->size), ax(num->ty->size))) {
                                fprintf(fp, "\t%s\t\t%s, %s\n", mov(num->ty->size), ax(num->ty->size), allocated_register(num->ty->size));
                                fprintf(fp, "\t%s\t\t%s\n", pop(), ax(8));
                            }
                        }
                            break;
                        default: {
                            fprintf(fp, "\t%s\t\t$%d, %s\n", fn(num->ty->size), num->val.ival, allocated_register(num->ty->size));
                        }
                            break;
                    }
                }
                    break;
                default: {
                    emit_expr(fp, expr->right);
                    
                    const char *reg = allocated_register(4);
                    unallocate_register();

                    switch (arith->kind) {
                        case ArithKind_Div: {
                            if (strcmp(allocated_register(4), ax(4))) {
                                fprintf(fp, "\t%s\t\t%s\n", push(), ax(8));
                                fprintf(fp, "\t%s\t\t%s, %s\n", mov(4), allocated_register(4), ax(4));
                            }
                            if (strcmp(reg, cx(4))) {
                                fprintf(fp, "\t%s\t\t%s\n", push(), cx(8));
                                fprintf(fp, "\t%s\t\t%s, %s\n", mov(4), reg, cx(4));
                            }

                            fprintf(fp, "\t%s\n", clt());
                            fprintf(fp, "\t%s\t\t%s\n", fn(4), cx(4)); 

                            if (strcmp(reg, cx(4))) {
                                fprintf(fp, "\t%s\t\t%s\n", pop(), cx(8));
                            }
                            if (strcmp(allocated_register(4), ax(4))) {
                                fprintf(fp, "\t%s\t\t%s, %s\n", mov(4), ax(4), allocated_register(4));
                                fprintf(fp, "\t%s\t\t%s\n", pop(), ax(8));
                            }
                        }
                            break;
                        default: {
                            fprintf(fp, "\t%s\t\t%s, %s\n", fn(4), reg, allocated_register(4));
                        }
                            break;
                    }
                }
                    break;
            }
        }
            break;
        case NodeKind_CompExpr: {
            struct ASTNodeCompExpr *comp = (struct ASTNodeCompExpr *)expr;
            switch (expr->left->kind) {
                case NodeKind_Number: {
                    struct ASTNodeNum *num = (struct ASTNodeNum *)expr->left;
                    fprintf(fp, "\t%s\t\t$%d, %s\n", mov(num->ty->size), num->val.ival, allocate_register(num->ty->size));
                }
                    break;
                case NodeKind_Variable: {
                    struct ASTNodeVar *var = (struct ASTNodeVar *)expr->left;
                    fprintf(fp, "\t%s\t\t%d(%s), %s\n", mov(var->sym->ty->size), var->sym->offset, bp(), allocate_register(var->sym->ty->size));
                }
                    break;
                default: {
                    emit_expr(fp, expr->left);
                }
                    break;
            }

            switch (expr->right->kind) {
                case NodeKind_Number: {
                    struct ASTNodeNum *num = (struct ASTNodeNum *)expr->right;
                    fprintf(fp, "\t%s\t\t$%d, %s\n", cmp(num->ty->size), num->val.ival, allocated_register(num->ty->size));
                }
                    break;
                case NodeKind_Variable: {
                    struct ASTNodeVar *var = (struct ASTNodeVar *)expr->right;
                    fprintf(fp, "\t%s\t\t%d(%s), %s\n", cmp(var->sym->ty->size), var->sym->offset, bp(), allocated_register(var->sym->ty->size));
                }
                    break;
                default: {
                    emit_expr(fp, expr->right);

                    const char *reg = allocated_register(4);
                    unallocate_register();

                    fprintf(fp, "\t%s\t\t%s, %s\n", cmp(4), allocated_register(4), reg);
                }
                    break;
            }
        }
            break;
        case NodeKind_LogicalExpr: {
            struct ASTNodeLogicalExpr *logicalExpr = (struct ASTNodeLogicalExpr *)expr;
            switch (logicalExpr->kind) {
                case LogicalKind_And: {
                    logicalExpr->ast.trues = unique_label();
                    logicalExpr->ast.falses = unique_label();

                    emit_expr(fp, logicalExpr->ast.left);
                    comp_zero(fp, logicalExpr->ast.left);

                    fprintf(fp, "\t%s\t\t\t%s\n", jmp(CompInstKindEqual, 0), logicalExpr->ast.falses);

                    if (logicalExpr->ast.left->trues) {
                        fprintf(fp, "%s:\n", logicalExpr->ast.left->trues);
                    }
                    emit_expr(fp, logicalExpr->ast.right);
                    comp_zero(fp, logicalExpr->ast.right);

                    fprintf(fp, "\t%s\t\t\t%s\n", jmp(CompInstKindEqual, 1), logicalExpr->ast.falses);
                }
                    break;
                case LogicalKind_Or: {
                    logicalExpr->ast.trues = unique_label();
                    logicalExpr->ast.falses = unique_label();

                    emit_expr(fp, logicalExpr->ast.left);
                    comp_zero(fp, logicalExpr->ast.left);

                    fprintf(fp, "\t%s\t\t\t%s\n", jmp(CompInstKindEqual, 1), logicalExpr->ast.trues);

                    if (logicalExpr->ast.left->falses) {
                        fprintf(fp, "%s:\n", logicalExpr->ast.left->falses);
                    }
                    emit_expr(fp, logicalExpr->ast.right);
                    comp_zero(fp, logicalExpr->ast.right);

                    fprintf(fp, "\t%s\t\t\t%s\n", jmp(CompInstKindEqual, 0), logicalExpr->ast.falses);
                }
                    break;
                case LogicalKind_Not: {

                }
                    break;
            }
        }
            break;
    }
}

void emit_stmt(FILE *fp, struct ASTNode *stmt) {
    if (!stmt) {
        return ;
    }

    switch (stmt->kind) {
        case NodeKind_CompoundStmt: {
            struct ASTNodeCompoundStmt *compoundStmt = (struct ASTNodeCompoundStmt *)stmt;
            if (compoundStmt->ast.left == NULL) {
                fprintf(fp, "\t%s\n", nop());
                return ;
            }

            struct ASTNodeList *stmt = ((struct ASTNodeList *)(compoundStmt->ast.left))->next;
            if (!stmt) {
                fprintf(fp, "\t%s\n", nop());
                return ;
            }
            while (stmt && stmt->node) {
                emit_stmt(fp, stmt->node);
                stmt = stmt->next;
            }
        }
            break;
        case NodeKind_ExprStmt: {
            if (stmt->left) {
                emit_expr(fp, stmt->left);
            }
        }
            break;
        case NodeKind_IfStmt: {
            struct ASTNodeIfStmt *ifstmt = (struct ASTNodeIfStmt *)stmt;
            if (!ifstmt->cond) {
                return ;
            }

            emit_expr(fp, ifstmt->cond);

            fprintf(fp, "%s:\n", ifstmt->cond->trues);
            emit_stmt(fp, ifstmt->then);
            unallocate_register();

            fprintf(fp, "%s:\n", ifstmt->cond->falses);
            if (ifstmt->els) {
                emit_stmt(fp, ifstmt->els);
            }
        }
            break;
        case NodeKind_VarDecl: {
            struct ASTNodeVarDecl *decl = (struct ASTNodeVarDecl *)stmt;
            struct ASTNodeList *varlist = ((struct ASTNodeList *)decl->var)->next;

            while (varlist && varlist->node) {
                struct ASTNodeVar *var = (struct ASTNodeVar *)varlist->node;

                if (is_const_expr(var)) {
                    fprintf(fp, "\t%s\t\t$%d, %d(%s)\n", mov(var->sym->ty->size), eval(var), var->sym->offset, bp());
                } else {
                    emit_expr(fp, var->ast.right);
                    fprintf(fp, "\t%s\t\t%s, %d(%s)\n", mov(var->sym->ty->size), ax(var->sym->ty->size), var->sym->offset, bp());
                }

                varlist = varlist->next;
            }
        }
            break;
        case NodeKind_Return: {
            if (stmt->left == NULL) {
                return ;
            }

            emit_expr(fp, stmt->left);
            unallocate_register();
            fprintf(fp, "\t%s\t\t\t.L.return.%d\n", jmp(CompInstKindNone, 1), func_index);
        }
            break;
    }
}

void assign_lvar_offsets(struct ASTNodeFunction *func) {
    if (func == NULL) {
        return ;
    }

    int gp = 0, bottom = 0, top = 16;

    if (func->ast.left) {
        struct ASTNodeList *paramlist = ((struct ASTNodeList *)func->ast.left)->next;
        while (paramlist && paramlist->node) {
            struct ASTNodeVar *var = (struct ASTNodeVar *)paramlist->node;
            if (gp < GP_MAX) {
                bottom += var->sym->ty->size;
                bottom = align_to(bottom, var->sym->ty->align);
                var->sym->offset = -bottom;
            } else {
                var->sym->offset = top;
                top += align_to(var->sym->ty->size, 8);
            }

            ++gp;
            paramlist = paramlist->next;
        }
    }

    if (func->sym->locals != NULL) {
        struct ASTNodeList *locals = ((struct ASTNodeList *)func->sym->locals)->next;
        while (locals && locals->node) {
            struct ASTNodeVar *var = (struct ASTNodeVar *)locals->node;
            if (!var) {
                locals = locals->next;
                continue;
            }
            bottom += var->sym->ty->size;
            bottom = align_to(bottom, var->sym->ty->align);
            var->sym->offset = -bottom;

            locals = locals->next;
        }
    }

    func->sym->stack_size = align_to(bottom, 16) ?: 16;
}

void store_gp(FILE *fp, struct ASTNodeList *params) {
    if (params == NULL) {
        return ;
    }
    
    int nargs = 0;
    struct ASTNodeList *p = params->next;
    while (p && p->node) {
        ++nargs;
        p = p->next;
    }

    int gp = nargs - 1;
    p = params->prev;
    while (p && p->node) {
        if (gp < GP_MAX) {
            if (p->node->kind == NodeKind_Variable) {
                struct ASTNodeVar *var = (struct ASTNodeVar *)p->node;
                fprintf(fp, "\t%s\t\t%s, %d(%s)\n", mov(var->sym->ty->size), generic(gp, var->sym->ty->size), var->sym->offset, bp());
            }
        } else {
            if (p->node->kind == NodeKind_Variable) {
                struct ASTNodeVar *var = (struct ASTNodeVar *)p->node;
                fprintf(fp, "\t%s\t\t%d(%s), %s\n", mov(var->sym->ty->size), var->sym->offset, bp(), ax(var->sym->ty->size));
            }
        }

        --gp;
        p = p->prev;
    }
}

void emit_text(FILE *fp, struct ASTNode *prog) {
    fprintf(fp, "\t.text\n");

    struct ASTNodeList *prog_list = ((struct ASTNodeList *)prog)->next;
    while (prog_list && prog_list->node) {
        if (prog_list->node->kind != NodeKind_Function) {
            prog_list = prog_list->next;
            continue;
        }

        struct ASTNodeFunction *func = (struct ASTNodeFunction *)prog_list->node;
        inner_index = 0;

        #ifdef __APPLE__
            fprintf(fp, "\t.globl\t\t_%s\n", func->name);
        #else
            fprintf(fp, "\t.globl\t\t%s\n", func->name);
        #endif
        #ifdef __linux__
            fprintf(fp, "\t.type\t\t%s, @function\n", func->name);
        #endif

        #ifdef __APPLE__
            fprintf(fp, "_%s:\n", func->name);
        #else
            fprintf(fp, "%s:\n", func->name);
        #endif

        assign_lvar_offsets(func);

        prologue(fp, func->sym->stack_size);

        store_gp(fp, func->ast.left);
        emit_stmt(fp, func->ast.right);

        epilogue(fp);
        fprintf(fp, "\n");

        ++func_index;
        prog_list = prog_list->next;
    }
    
}

void emit_data(FILE *fp, struct ASTNode *prog) {
    fprintf(fp, "\t.data\n");
    struct ASTNodeList *prog_list = ((struct ASTNodeList *)prog)->next;
    while (prog_list && prog_list->node) {
        if (prog_list->node->kind != NodeKind_VarDecl) {
            prog_list = prog_list->next;
            continue;
        }

        struct ASTNodeVarDecl *decl = (struct ASTNodeVarDecl *)prog_list->node;
        struct ASTNodeList *varlist = ((struct ASTNodeList *)decl->var)->next;
        while (varlist && varlist->node) {
            struct ASTNodeVar *var = (struct ASTNodeVar *)varlist->node;
            struct Type *ty = (var->sym->ty?: decl->ty);

            fprintf(fp, "\t.globl\t\t%s\n", var->name);
            #ifdef __linux__
                fprintf(fp, "\t.type\t\t%s, @object\n", var->name);
            #endif
            fprintf(fp, "\t.align\t\t%d\n", ty->align);
            fprintf(fp, "%s:\n", var->name);

            if (var->ast.right == NULL) {
                fprintf(fp, "\t.zero\t\t%d\n", align_to(ty->size, ty->align));
            } else {
                switch (var->ast.right->kind) {
                    case NodeKind_Number: {
                        struct ASTNodeNum *num = (struct ASTNodeNum *)var->ast.right;
                        fprintf(fp, "   \t.long\t\t%llu\n", num->val.ival);
                    }
                        break;
                }
            }
            fprintf(fp, "\n");

            varlist = varlist->next;
        }

        prog_list = prog_list->next;
    }
}

void emit_prog(FILE *fp, struct ASTNode *prog) {
    if (prog == NULL || fp == NULL) {
        return ;
    }

    emit_text(fp, prog);

    emit_data(fp, prog);
}

void emit(struct Context *context) {
    if (!context) {
        return ;
    }

    emit_prog(context->output, context->prog);
}