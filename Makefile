CC=gcc
CFLAGS=-Wall -g

all: aps

parser: aps.y
	bison -d aps.y

lexer: aps.l
	flex aps.l

aps: lexer parser prolog_gen.c ast.c eval.c 
	$(CC) $(CFLAGS) -o aps aps.c eval.c prolog_gen.c ast.c aps.tab.c lex.yy.c -lfl

clean:
	rm -f *tab.* lex.yy.c aps *~ *.o tests
