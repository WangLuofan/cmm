OBJS=ast.o type.o codegen.o utils.o hashmap.o frame.o instruction.o semantic.o eval.o symbol.o scope.o

cmm: main.c cmm.tab.c cmm.lex.c $(OBJS)
	gcc -g -o $@ main.c cmm.tab.c cmm.lex.c -lfl $(OBJS)

cmm.tab.c: cmm.y
	bison -d -v cmm.y

cmm.lex.c: cmm.l
	flex -o $@ cmm.l

type.o: type.h type.c
	gcc -g -c -o $@ type.c

ast.o: ast.h type.h ast.c
	gcc -g -c -o $@ ast.c

codegen.o: ast.h utils.h codegen.c
	gcc -g -c -o $@ codegen.c

utils.o: utils.c utils.h
	gcc -g -c -o $@ utils.c

eval.o: utils.h ast.h eval.h eval.c
	gcc -g -c -o $@ eval.c

hashmap.o: hashmap.c hashmap.h
	gcc -g -c -o $@ hashmap.c

frame.o: frame.h frame.c
	gcc -g -c -o $@ frame.c

instruction.o: instruction.h instruction.c
	gcc -g -c -o $@ instruction.c

semantic.o: semantic.h semantic.c
	gcc -g -c -o $@ semantic.c

symbol.o: symbol.h symbol.c
	gcc -g -c -o $@ symbol.c

scope.o: scope.h scope.c
	gcc -g -c -o $@ scope.c

clean:
	rm -rf cmm.lex.c cmm.tab.h cmm.tab.c cmm *.o *.s *.output a.out

.PHONY: clean
