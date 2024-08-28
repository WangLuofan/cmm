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