#ifndef __TYPE_H__
#define __TYPE_H__

typedef enum TypeKind {
    TypeKind_Void,
    TypeKind_Int
}TypeKind;

typedef struct Type {
    TypeKind kind;

    int size;
    int align;
    int is_signed;
}Type;

extern struct Type *ty_int;

#endif