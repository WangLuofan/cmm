cmm: cmm.c cmm.tab.c cmm.lex.c
	gcc -g -o $@ cmm.c cmm.tab.c cmm.lex.c -lfl

cmm.tab.c: cmm.y
	bison -d cmm.y

cmm.lex.c: cmm.l
	flex -o $@ cmm.l

clean:
	rm -rf cmm.lex.c cmm.tab.h cmm.tab.c cmm

.PHONY: clean