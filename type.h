#ifndef __TYPE_H__
#define __TYPE_H__

typedef enum TypeKind {
    TypeKind_Void,
    TypeKind_Int,
    TypeKind_Char,
    TypeKind_Short,
    TypeKind_Long
}TypeKind;

typedef struct Type {
    TypeKind kind;

    int size;
    int align;
    int is_signed;
}Type;

struct Type *copy_type(struct Type *);

extern struct Type *ty_void;
extern struct Type *ty_int;
extern struct Type *ty_char;
extern struct Type *ty_short;
extern struct Type *ty_long;

#endif