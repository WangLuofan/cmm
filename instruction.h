#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

const char *mov(int);
const char *nop(void);
const char *call(void);
const char *ret(void);
const char *push(void);
const char *pop(void);
const char *sub(int);
const char *add(int);
const char *mul(int);
const char *div(int);
const char *clt(int);

#endif