TESTDIR = tests
PREFIX = parser

all : README.md $(PREFIX) $(TESTDIR) $(TESTDIR)/tests

$(PREFIX) : main.o y.tab.o lex.yy.o ast.o symboles.o
	gcc $^ -ly -lfl -o $(PREFIX)

y.tab.o: $(PREFIX).y
	yacc -d $(PREFIX).y
	gcc -c y.tab.c

lex.yy.o: $(PREFIX).l y.tab.h
	lex $(PREFIX).l
	gcc -c lex.yy.c

%.o: %.c
	gcc -c $*.c



README.md : ;

$(TESTDIR)/tests : $(TESTDIR)/tests.o y.tab.o lex.yy.o ast.o symboles.o
	gcc -o $(TESTDIR)/tests $(TESTDIR)/tests.o y.tab.o lex.yy.o ast.o symboles.o -ly -lfl -l cmocka -L /usr/local/lib

$(TESTDIR) : 
	mkdir -p $(TESTDIR)

$(TESTDIR)/%.o : $(TESTDIR)/%.c
	gcc -c $^ -o $(TESTDIR)/$*.o -l cmocka -L /usr/local/lib

.PHONY: test clean
test :
	./$(TESTDIR)/tests
	valgrind --tool=memcheck --undef-value-errors=no --error-exitcode=1 --leak-resolution=high --leak-check=full --quiet --child-silent-after-fork=yes ./$(PREFIX)

clean :
	rm -f $(TESTDIR)/*.o $(TESTDIR)/tests
	rm -f *.o y.tab.c y.tab.h lex.yy.c a.out $(PREFIX)
