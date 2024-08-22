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

            struct ASTNodeVar *var = (struct ASTNodeVar *)decl->ast.left;
            fprintf(fp, "\t.globl %s\n", var->name);
            fprintf(fp, "\t.type %s, @object\n");
            fprintf(fp, "\t.align %d\n", decl->ty->align);
            fprintf(fp, "%s:\n", var->name);

            if (decl->ast.right == NULL) {
                fprintf(fp, "\t.zero %d\n", align_to(decl->ty->size, decl->ty->align));
            }
            fprintf(fp, "\n");
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