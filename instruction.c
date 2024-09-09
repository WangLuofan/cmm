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

    return "movl";
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

    return "subl";
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

    return "addl";
}

const char *mul(int sz) {
    switch (sz) {
        case 1:
            return "imulb";
        case 2:
            return "imulw";
        case 4:
            return "imull";
        case 8:
            return "imulq";
    }

    return "imull";
}

const char *div(int sz) {
    switch (sz) {
        case 1:
            return "idivb";
        case 2:
            return "idivw";
        case 4:
            return "idivl";
        case 8:
            return "idivq";
    }

    return "idivl";
}

const char *clt(int sz) {
    return "cltd";
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