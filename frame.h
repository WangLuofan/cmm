#ifndef __FRAME_H__
#define __FRAME_H__

#define GP_MAX 6

const char *ax(int);
const char *bx(int);
const char *cx(int);
const char *dx(int);
const char *di(int);
const char *si(int);
const char *r8(int);
const char *r9(int);

const char *sp(void);
const char *bp(void);

const char *generic(int, int);

const char *allocate_register(int);
const char *allocated_register(int);

void unallocate_register(void);
void unallocate_all(void);

#endif