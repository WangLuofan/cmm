#include "ast.h"
#include "utils.h"
#include "context.h"
#include "codegen.h"

void prologue(FILE *fp, int stack_size) {
    fprintf(fp, "\tendbr64\n");
    fprintf(fp, "\tpushq %%rbp\n");
    fprintf(fp, "\tmovq %%rsp, %%rbp\n");

    if (stack_size) {
        fprintf(fp, "\tsubq $%d, %%rbp\n", align_to(stack_size, 16));
    }
}

void epilogue(FILE *fp) {
    fprintf(fp, "\tpopq %%rbp\n");
    fprintf(fp, "\tretq\n");
}

void emit_stmt(FILE *fp, struct ASTNode *stmts) {
    if (stmts == NULL) {
        fprintf(fp, "\tnop\n");
        return ;
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
        fprintf(fp, "\t.globl %s\n", func->name);
        fprintf(fp, "\t.type %s, @function\n", func->name);
        fprintf(fp, "%s:\n", func->name);

        prologue(fp, 0);

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

            fprintf(fp, "\t.globl %s\n", var->name);
            fprintf(fp, "\t.type %s, @object\n", var->name);
            fprintf(fp, "\t.align %d\n", var->ty->align);
            fprintf(fp, "%s:\n", var->name);

            if (var->val == NULL) {
                fprintf(fp, "\t.zero %d\n", align_to(var->ty->size, var->ty->align));
            } else {
                switch (var->val->kind) {
                    case NodeKind_Number: {
                        struct ASTNodeNum *num = (struct ASTNodeNum *)var->val;
                        fprintf(fp, "   \t.long %llu\n", num->ival);
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