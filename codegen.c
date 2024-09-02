#include "ast.h"
#include "eval.h"
#include "utils.h"
#include "frame.h"
#include "context.h"
#include "codegen.h"
#include "instruction.h"

void prologue(FILE *fp, int stack_size) {
    fprintf(fp, "\tendbr64\n");
    fprintf(fp, "\t%s\t\t%s\n", push(), sp());
    fprintf(fp, "\t%s\t\t%s, %s\n", mov(8), sp(), bp());

    if (stack_size) {
        fprintf(fp, "\tsubq\t\t$%d, %s\n", align_to(stack_size, 16), bp());
    }
}

void epilogue(FILE *fp) {
    fprintf(fp, "\t%s\t\t%s\n", pop(), bp());
    fprintf(fp, "\t%s\n", ret());
}

void emit_expr(FILE *fp, struct ASTNode *expr) {
    if (!expr) {
        return ;
    }

    switch (expr->kind) {
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

            if (args && args->node) {
                struct ASTNodeList *prev = ((struct ASTNodeList *)(((struct ASTNodeFnCall *)expr)->ast.left))->prev;
                while (prev && prev->node && prev != args->prev) {
                    if (is_const_expr(prev->node)) {
                        int res = eval(prev->node);
                        fprintf(fp, "\t%s\t\t$%d\n", push(), res);
                    } else {
                        eval(prev->node);
                        fprintf(fp, "\t%s\t\t%s\n", push(), ax(4));
                    }

                    prev = prev->prev;
                }
            }

            fprintf(fp, "\t%s\t\t%s\n", call(), ((struct ASTNodeFnCall *)expr)->name);
        }
            break;
        case NodeKind_Number: {
            struct ASTNodeNum *num = (struct ASTNodeNum *)expr;
            fprintf(fp, "\t%s\t\t$%d, %s\n", mov(sizeof(num->value.ival)), num->value.ival, ax(sizeof(num->value.ival)));
        }
            break;
    }
}

void emit_stmt(FILE *fp, struct ASTNode *stmts) {
    switch (stmts->kind) {
        case NodeKind_CompoundStmt: {
            struct ASTNodeList *stmt = ((struct ASTNodeList *)(((struct ASTNodeCompoundStmt *)stmts)->ast.left))->next;
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
            struct ASTNode *node = ((struct ASTNode *)stmts)->left;
            if (node) {
                emit_expr(fp, node);
            }
        }
            break;
        case NodeKind_VarDecl: {
            struct ASTNodeVarDecl *decl = (struct ASTNodeVarDecl *)stmts;
            struct ASTNodeList *varlist = ((struct ASTNodeList *)decl->var)->next;

            while (varlist && varlist->node) {
                struct ASTNodeVar *var = (struct ASTNodeVar *)varlist->node;

                if (is_const_expr(var)) {
                    fprintf(fp, "\t%s\t\t$%d, %d(%s)\n", mov(var->ty->size), eval(var), var->offset, bp());
                } else {
                    emit_expr(fp, var->ast.right);
                    fprintf(fp, "\t%s\t\t%s, %d(%s)\n", mov(var->ty->size), ax(var->ty->size), var->offset, bp());
                }

                varlist = varlist->next;
            }
        }
            break;
    }
}

void assign_lvar_offsets(struct ASTNodeFunction *func) {
    if (func == NULL || func->locals == NULL) {
        return ;
    }

    int gp = 0, bottom = 0, top = 16;

    struct ASTNodeList *paramlist = ((struct ASTNodeList *)func->ast.left)->next;
    while (paramlist && paramlist->node) {
        struct ASTNodeVar *var = (struct ASTNodeVar *)paramlist->node;
        if (gp < GP_MAX) {
            bottom += var->ty->size;
            bottom = align_to(bottom, var->ty->align);
            var->offset = -bottom;
        } else {
            var->offset = top;
            top += align_to(var->ty->size, 8);
        }

        ++gp;
        paramlist = paramlist->next;
    }

    struct ASTNodeList *locals = ((struct ASTNodeList *)func->locals)->next;
    while (locals && locals->node) {
        struct ASTNodeVar *var = (struct ASTNodeVar *)locals->node;
        if (!var) {
            locals = locals->next;
            continue;
        }
        bottom += var->ty->size;
        bottom = align_to(bottom, var->ty->align);
        var->offset = -bottom;

        locals = locals->next;
    }

    func->stack_size = align_to(bottom, 16);
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
                fprintf(fp, "\t%s\t\t%s, %d(%s)\n", mov(var->ty->size), generic(gp, var->ty->size), var->offset, bp());
            }
        } else {
            if (p->node->kind == NodeKind_Variable) {
                struct ASTNodeVar *var = (struct ASTNodeVar *)p->node;
                fprintf(fp, "\t%s\t\t%d(%s), %s\n", mov(var->ty->size), var->offset, bp(), ax(var->ty->size));
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

        fprintf(fp, "\t.globl\t\t%s\n", func->name);
        fprintf(fp, "\t.type\t\t%s, @function\n", func->name);
        fprintf(fp, "%s:\n", func->name);

        assign_lvar_offsets(func);

        prologue(fp, func->stack_size);

        store_gp(fp, func->ast.left);
        emit_stmt(fp, func->ast.right);

        epilogue(fp);
        fprintf(fp, "\n");

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
            struct Type *ty = (var->ty?: decl->ty);

            fprintf(fp, "\t.globl\t\t%s\n", var->name);
            fprintf(fp, "\t.type\t\t%s, @object\n", var->name);
            fprintf(fp, "\t.align\t\t%d\n", ty->align);
            fprintf(fp, "%s:\n", var->name);

            if (var->ast.right == NULL) {
                fprintf(fp, "\t.zero\t\t%d\n", align_to(ty->size, ty->align));
            } else {
                switch (var->ast.right->kind) {
                    case NodeKind_Number: {
                        struct ASTNodeNum *num = (struct ASTNodeNum *)var->ast.right;
                        fprintf(fp, "   \t.long\t\t%llu\n", num->value.ival);
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