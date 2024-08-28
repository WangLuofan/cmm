#include "ast.h"
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
            struct ASTNode *call_exp = expr->left;

            int gp = 0, offset = 0;
            while (call_exp) {
                struct ASTNodeNum *num = NULL;
                if (call_exp->kind == NodeKind_Number) {
                    num = (struct ASTNodeNum *)call_exp;
                } else if (call_exp->kind == NodeKind_CommaExpr) {
                    if (call_exp->left->kind == NodeKind_Number) {
                        num = (struct ASTNodeNum *)call_exp->left;
                    } else {
                        emit_expr(fp, call_exp->left);
                    }
                }

                if (gp < GP_MAX) {
                    if (num) {
                        fprintf(fp, "\t%s\t\t$%d, %s\n", mov(sizeof(num->value.ival)), num->value.ival, generic(gp, sizeof(num->value.ival)));
                    } else {
                        fprintf(fp, "\t%s\t\t%s, %s\n", mov(sizeof(num->value.ival)), ax(sizeof(num->value.ival)), generic(gp, sizeof(num->value.ival)));
                    }
                } else {
                    if (num) {
                        fprintf(fp, "\t%s\t\t$%d, %d(%s)\n", mov(sizeof(num->value.ival)), num->value.ival, offset, bp());
                        offset += 8;
                    }
                }

                call_exp = call_exp->right;
                ++gp;
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
            struct ASTNodeList *stmt = ((struct ASTNodeCompoundStmt *)stmts)->ast.left;
            if (!stmt) {
                fprintf(fp, "\t%s\n", nop());
                return ;
            }
            while (stmt) {
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
    }
}

void assign_lvar_offsets(struct ASTNodeFunction *func) {
    if (func == NULL) {
        return ;
    }

    struct ASTNodeList *paramlist = (struct ASTNodeList *)func->ast.left;

    int gp = 0, bottom = 0, top = 16;
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
}

void store_gp(FILE *fp, struct ASTNodeList *params) {
    struct ASTNodeList *p = (struct ASTNodeList *)params;

    int gp = 0;
    while (p && p->node) {
        if (gp >= GP_MAX) {
            break;
        }

        if (p->node->kind == NodeKind_Variable) {
            struct ASTNodeVar *var = (struct ASTNodeVar *)p->node;
            fprintf(fp, "\t%s\t\t%s, %d(%s)\n", mov(var->ty->size), generic(gp, var->ty->size), var->offset, bp());
        }

        ++gp;
        p = p->next;
    }
}

void emit_text(FILE *fp, struct ASTNode *prog) {
    fprintf(fp, "\t.text\n");

    struct ASTNodeList *prog_list = (struct ASTNodeList *)prog;
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

        prologue(fp, 0);

        store_gp(fp, func->ast.left);
        emit_stmt(fp, func->ast.right);

        epilogue(fp);
        fprintf(fp, "\n");

        prog_list = prog_list->next;
    }
    
}

void emit_data(FILE *fp, struct ASTNode *prog) {
    fprintf(fp, "\t.data\n");
    struct ASTNodeList *prog_list = (struct ASTNodeList *)prog;
    while (prog_list && prog_list->node) {
        if (prog_list->node->kind == NodeKind_Function) {
            prog_list = prog_list->next;
            continue;
        }

        struct ASTNodeList *varlist = (struct ASTNodeList *)prog_list->node;
        while (varlist && varlist->node) {
            struct ASTNodeVar *var = (struct ASTNodeVar *)varlist->node;

            fprintf(fp, "\t.globl\t\t%s\n", var->name);
            fprintf(fp, "\t.type\t\t%s, @object\n", var->name);
            fprintf(fp, "\t.align\t\t%d\n", var->ty->align);
            fprintf(fp, "%s:\n", var->name);

            if (var->val == NULL) {
                fprintf(fp, "\t.zero\t\t%d\n", align_to(var->ty->size, var->ty->align));
            } else {
                switch (var->val->kind) {
                    case NodeKind_Number: {
                        struct ASTNodeNum *num = (struct ASTNodeNum *)var->val;
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