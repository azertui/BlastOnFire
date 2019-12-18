TESTDIR = tests
PREFIX = parser

#All
all : README.md $(PREFIX) $(TESTDIR) $(TESTDIR)/tests

#Executable principal (nom : variable PREFIX)
$(PREFIX) : main.o y.tab.o lex.yy.o ast.o symboles.o
	gcc $^ -ly -lfl -o $@

#Fichier objet YACC
y.tab.o: $(PREFIX).y
	yacc -d $(PREFIX).y
	gcc -c y.tab.c

#Fichier objet LEX
lex.yy.o: $(PREFIX).l y.tab.h
	lex $(PREFIX).l
	gcc -c lex.yy.c

#Fichiers objets classiques
%.o: %.c
	gcc -c $*.c



README.md : ;

#Executable de test
$(TESTDIR)/tests : $(TESTDIR)/tests.o y.tab.o lex.yy.o ast.o symboles.o
	gcc -o $@ $^ -ly -lfl -l cmocka -L /usr/local/lib

#Dossier de test
$(TESTDIR) : 
	mkdir -p $(TESTDIR)

#Fichiers objets de test
$(TESTDIR)/%.o : $(TESTDIR)/%.c $(TESTDIR)
	gcc -c $< -o $@ -l cmocka -L /usr/local/lib

#Phony
.PHONY: test clean

#Cible de test
test : $(TESTDIR)/tests
	./$(TESTDIR)/tests
	valgrind --tool=memcheck --undef-value-errors=no --error-exitcode=1 --leak-resolution=high --leak-check=full --quiet --child-silent-after-fork=yes ./$(PREFIX)

#Clean
clean :
	rm -f $(TESTDIR)/*.o $(TESTDIR)/tests
	rm -f *.o y.tab.c y.tab.h lex.yy.c a.out $(PREFIX)
