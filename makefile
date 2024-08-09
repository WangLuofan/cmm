cmm: main.c cmm.tab.c cmm.yy.c ast.o codegen.o
	gcc -g main.c cmm.tab.c cmm.yy.c ast.o codegen.o -lfl -o cmm

cmm.tab.c: cmm.y
	bison -d cmm.y

cmm.yy.c: cmm.tab.h cmm.l
	flex -o cmm.yy.c cmm.l

ast.o: ast.c
	gcc -g -c -o $@ ast.c

codegen.o: codegen.c
	gcc -g -c -o $@ codegen.c

clean:
	rm -rf cmm cmm.yy.c cmm.tab.h cmm.tab.c *.o