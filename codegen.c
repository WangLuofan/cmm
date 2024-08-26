#include "ast.h"
#include "utils.h"
#include "context.h"
#include "codegen.h"

#define GP_MAX 6

static const char *reg8[] = {"%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b"};
static const char *reg16[] = {"%di", "%si", "%dx", "%cx", "%r8w", "%r9w"};
static const char *reg32[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
static const char *reg64[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

void prologue(FILE *fp, int stack_size) {
    fprintf(fp, "\tendbr64\n");
    fprintf(fp, "\tpushq\t\t%%rbp\n");
    fprintf(fp, "\tmovq\t\t%%rsp, %%rbp\n");

    if (stack_size) {
        fprintf(fp, "\tsubq\t\t$%d, %%rbp\n", align_to(stack_size, 16));
    }
}

void epilogue(FILE *fp) {
    fprintf(fp, "\tpopq\t\t%%rbp\n");
    fprintf(fp, "\tretq\n");
}

void emit_stmt(FILE *fp, struct ASTNode *stmts) {
    if (stmts == NULL) {
        fprintf(fp, "\tnop\n");
        return ;
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
            bottom += align_to(var->ty->size, var->ty->align);
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
            switch (var->ty->size) {
                case 1: {

                }
                    break;
                case 2: {

                }
                    break;
                case 4: {
                    fprintf(fp, "\tmovl\t\t%s, %d(%%rbp)\n", reg32[gp], var->offset);
                }
                    break;
                case 8: {

                }
                    break;
                default: {

                }
                    break;
            }
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
                        fprintf(fp, "   \t.long\t\t%llu\n", num->ival);
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