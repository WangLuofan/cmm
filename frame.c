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