all: compiler

compiler: main.o ast.o semantic.o icg.o co.o tcg.o lex.yy.o parser.tab.o
	gcc -o compiler main.o ast.o semantic.o icg.o co.o tcg.o lex.yy.o parser.tab.o -ll

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

main.o: main.c
	gcc -c main.c

ast.o: ast.c
	gcc -c ast.c

semantic.o: semantic.c
	gcc -c semantic.c

icg.o: icg.c
	gcc -c icg.c

co.o: co.c
	gcc -c co.c

tcg.o: tcg.c
	gcc -c tcg.c

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h compiler output.c