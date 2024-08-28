#include <stdlib.h>
#include "type.h"

struct Type *ty_int = &(struct Type) { TypeKind_Int, 4, 4, 1 };
struct Type *ty_void = &(struct Type) { TypeKind_Void, 1, 1, 1 };
struct Type *ty_char = &(struct Type) { TypeKind_Char, 1, 1, 1 };
struct Type *ty_short = &(struct Type) { TypeKind_Short, 2, 2, 1};
struct Type *ty_long = &(struct Type) { TypeKind_Long, 8, 8, 1 }; 

struct Type *copy_type(struct Type *ty) {
    if (ty == NULL) {
        return NULL;
    }

    struct Type *copy_ty = (struct Type *)malloc(sizeof(struct Type));
    *copy_ty = *ty;

    return copy_ty;
}