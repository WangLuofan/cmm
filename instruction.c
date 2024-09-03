#include "instruction.h"

const char *mov(int sz) {
    switch (sz) {
        case 1:
            return "movb";
        case 2:
            return "movw";
        case 4:
            return "movl";
        case 8:
            return "movq";
    }

    return "movq";
}

const char *sub(int sz) {
    switch (sz)
    {
    case 1:
        return "subb";
    case 2:
        return "subw";
    case 4:
        return "subl";
    case 8:
        return "subq";
    }

    return "subq";
}

const char *add(int sz) {
    switch (sz)
    {
    case 1:
        return "addb";
    case 2:
        return "addw";
    case 4:
        return "addl";
    case 8:
        return "addq";
    }

    return "addq";
}

const char *nop(void) {
    return "nop";
}

const char *call(void) {
    return "callq";
}

const char *ret(void) {
    return "retq";
}

const char *push(void) {
    return "pushq";
}
const char *pop(void) {
    return "popq";
}