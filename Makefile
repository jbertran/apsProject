CC=gcc
CFLAGS=-Wall -g

all: eval type

parser: aps.y
	bison -d aps.y

lexer: aps.l
	flex aps.l

type: lexer parser prolog_gen.c typeur.c ast.c eval.c
	$(CC) $(CFLAGS) -o type typeur.c prolog_gen.c ast.c aps.tab.c lex.yy.c -lfl

eval: lexer parser prolog_gen.c aps.c ast.c eval.c 
	$(CC) $(CFLAGS) -o eval aps.c eval.c ast.c aps.tab.c lex.yy.c -lfl

clean:
	rm -f *tab.* lex.yy.c eval type *~ *.o tests	
