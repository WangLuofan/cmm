#include "frame.h"
#include <stdio.h>

const char *ax(int sz) {
    switch (sz) {
        case 1:
            return "%al";
        case 2:
            return "%ax";
        case 4:
            return "%eax";
        case 8:
            return "%rax";
    }

    return NULL;
}

const char *bx(int sz) {
    switch (sz) {
        case 1:
            return "%bl";
        case 2:
            return "%bx";
        case 4:
            return "%ebx";
        case 8:
            return "%rbx";
    }

    return NULL;
}

const char *cx(int sz) {
    switch (sz) {
        case 1:
            return "%cl";
        case 2:
            return "%cx";
        case 4:
            return "%ecx";
        case 8:
            return "%rcx";
    }

    return NULL;
}

const char *dx(int sz) {
    switch (sz) {
        case 1:
            return "%dl";
        case 2:
            return "%dx";
        case 4:
            return "%edx";
        case 8:
            return "%rdx";
    }

    return NULL;
}

const char *di(int sz) {
    switch (sz) {
        case 1:
            return "%dil";
        case 2:
            return "%di";
        case 4:
            return "%edi";
        case 8:
            return "%rdi";
    }

    return NULL;
}

const char *si(int sz) {
    switch (sz) {
        case 1:
            return "%sil";
        case 2:
            return "%si";
        case 4:
            return "%esi";
        case 8:
            return "%rsi";
    }

    return NULL;
}

const char *r8(int sz) {
    switch (sz) {
        case 1:
            return "%r8b";
        case 2:
            return "%r8w";
        case 4:
            return "%r8d";
        case 8:
            return "%r8";
    }

    return NULL;
}

const char *r9(int sz) {
    switch (sz) {
        case 1:
            return "%r9b";
        case 2:
            return "%r9w";
        case 4:
            return "%r9d";
        case 8:
            return "%r9";
    }

    return NULL;
}

const char *r10(int sz) {
    switch (sz) {
        case 1:
            return "%r10b";
        case 2:
            return "%r10w";
        case 4:
            return "%r10d";
        case 8:
            return "%r10";
    }

    return NULL;
}

const char *r11(int sz) {
    switch (sz) {
        case 1:
            return "%r11b";
        case 2:
            return "%r11w";
        case 4:
            return "%r11d";
        case 8:
            return "%r11";
    }

    return NULL;
}

const char *sp(void) {
    return "%rsp";   
}

const char *bp(void) {
    return "%rbp";
}

const char *generic(int gp, int sz) {
    const char *reg[] = { di(sz), si(sz), dx(sz), cx(sz), r8(sz), r9(sz) };
    return reg[gp];
}

const char *reg(int index, int sz) {
    const char *reg[] = { ax(sz), cx(sz), dx(sz), di(sz), si(sz), r8(sz), r9(sz), r10(sz), r11(sz) };
    return reg[index];
}

static int reg_index = 0;
static int reg_stack[9] = { 0 };

const char *allocate_register(int sz) {
    reg_stack[reg_index] = reg_index + 1;
    return reg(reg_index++, sz);
}

const char *allocated_register(int sz) {
    if (reg_index <= 0) {
        return NULL;
    }

    return reg(reg_stack[reg_index - 1] - 1, sz);
}

void unallocate_register(void) {
    reg_stack[--reg_index] = 0;
}