all:
	bison -d parser.y
	flex lexer.l
	gcc parser.tab.c lex.yy.c ast.c semantic.c icg.c co.c tcg.c main.c -o compiler -ll

clean:
	rm -f compiler parser.tab.c parser.tab.h lex.yy.c output.c
