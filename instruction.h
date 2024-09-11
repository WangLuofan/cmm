#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

typedef enum CompInstKind {
    CompInstKindNone,
    CompInstKindLessThan,
    CompInstKindLessEqual,
    CompInstKindEqual,
    CompInstKindNotEqual,
    CompInstKindGreaterThan,
    CompInstKindGreaterEqual
}CompInstKind;

const char *mov(int);
const char *nop(void);
const char *call(void);
const char *ret(void);
const char *push(void);
const char *pop(void);
const char *isub(int);
const char *iadd(int);
const char *imul(int);
const char *idiv(int);
const char *clt(void);
const char *cmp(int);
const char *jmp(CompInstKind);

#endif