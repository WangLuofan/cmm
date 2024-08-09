#include "types.h"

typedef struct Type {
    TypeKind kind;
    int size;
}Type;

struct Type Ty_int = { TypeKind_Int, sizeof(int) };