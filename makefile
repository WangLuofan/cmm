cmm: main.c cmm.tab.c cmm.yy.c
	gcc -g main.c cmm.tab.c cmm.yy.c -lfl -o cmm

cmm.tab.c: cmm.y
	bison -d cmm.y

cmm.yy.c: cmm.tab.h cmm.l
	flex -o cmm.yy.c cmm.l

clean:
	rm -rf cmm cmm.yy.c cmm.tab.h cmm.tab.c
