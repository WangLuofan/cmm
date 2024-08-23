#include "ast.h"
#include "utils.h"
#include "context.h"
#include "codegen.h"

void emit_prog(FILE *fp, struct ASTNode *prog) {
    if (!prog) {
        return ;
    }

    switch (prog->kind) {
        case NodeKind_Decl: {
            struct ASTNodeDecl *decl = (struct ASTNodeDecl *)prog;

            struct ASTNodeList *varlist = (struct ASTNodeList *)decl->ast.left;
            while (varlist && varlist->node) {
                struct ASTNodeVar *var = (struct ASTNodeVar *)varlist->node;

                fprintf(fp, "\t.globl %s\n", var->name);
                fprintf(fp, "\t.type %s, @object\n");
                fprintf(fp, "\t.align %d\n", decl->ty->align);
                fprintf(fp, "%s:\n", var->name);

                if (var->val == NULL) {
                    fprintf(fp, "\t.zero %d\n", align_to(decl->ty->size, decl->ty->align));
                } else {
                    switch (var->val->kind) {
                        case NodeKind_Number: {
                            struct ASTNodeNum *num = (struct ASTNodeNum *)var->val;
                            fprintf(fp, "   \t.long %d\n", num->ival);
                        }
                            break;
                    }
                }
                fprintf(fp, "\n");

                varlist = varlist->next;
            }
        }
            break;
    }
}

void emit(struct Context *context) {
    if (!context) {
        return ;
    }

    emit_prog(context->output, context->prog);
}