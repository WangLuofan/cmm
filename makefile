OBJS=ast.o type.o 

cmm: cmm.c cmm.tab.c cmm.lex.c $(OBJS)
	gcc -g -o $@ cmm.c cmm.tab.c cmm.lex.c -lfl $(OBJS)

cmm.tab.c: cmm.y
	bison -d cmm.y

cmm.lex.c: cmm.l
	flex -o $@ cmm.l

type.o: type.h type.c
	gcc -g -c -o $@ type.c

ast.o: ast.h type.h ast.c
	gcc -g -c -o $@ ast.c

clean:
	rm -rf cmm.lex.c cmm.tab.h cmm.tab.c cmm *.o

.PHONY: clean
