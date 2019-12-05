prefixe=expression

all: $(prefixe).y $(prefixe).l
	yacc -d $(prefixe).y
	lex $(prefixe).l
	gcc y.tab.c lex.yy.c -ly -ll

clean:
	rm *.o y.tab.c y.tab.h lex.yy.c a.out
