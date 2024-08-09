#ifndef __TYPES_H__
#define __TYPES_H__

typedef enum TypeKind {
    TypeKind_Void,
    TypeKind_Char,
    TypeKind_Short,
    TypeKind_Int,
    TypeKind_Long,
    TypeKind_LongLong,
    TypeKind_Float,
    TypeKind_Double,
    TypeKind_LongDouble
}TypeKind;

typedef struct Type;

#endif