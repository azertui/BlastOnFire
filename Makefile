TESTDIR = tests
PREFIX = parser

#All
all : README.md $(PREFIX) $(TESTDIR) $(TESTDIR)/tests

<<<<<<< HEAD
$(PREFIX) : y.tab.o lex.yy.o main.o ast.o symboles.o
	gcc main.o y.tab.o lex.yy.o ast.o symboles.o -ly -ll -o $(PREFIX)

main.o: main.c
	gcc -c main.c

ast.o: ast.c
	gcc -c ast.c

symboles.o: symboles.c
	gcc -c symboles.c
=======
#Executable principal (nom : variable PREFIX)
$(PREFIX) : main.o y.tab.o lex.yy.o ast.o symboles.o
	gcc $^ -ly -lfl -o $@
>>>>>>> master

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
<<<<<<< HEAD
	gcc -o $(TESTDIR)/tests $(TESTDIR)/tests.o y.tab.o lex.yy.o ast.o symboles.o -ly -ll -l cmocka -L /usr/local/lib
=======
	gcc -o $@ $^ -ly -lfl -l cmocka -L /usr/local/lib

#Dossier de test
>>>>>>> master
$(TESTDIR) : 
	mkdir -p $(TESTDIR)

#Fichiers objets de test
$(TESTDIR)/%.o : $(TESTDIR)/%.c $(TESTDIR)
	gcc -c $< -o $@ -l cmocka -L /usr/local/lib

#Phony
.PHONY: test clean doxygen

#Cible de test
test : $(TESTDIR)/tests
	./$(TESTDIR)/tests 2>/dev/null | grep "\[.*\]" 
	valgrind --tool=memcheck --undef-value-errors=no --error-exitcode=1 --leak-resolution=high --leak-check=full --quiet --child-silent-after-fork=yes ./$(PREFIX) >/dev/null

#Clean
clean :
	rm -f $(TESTDIR)/*.o $(TESTDIR)/tests
	rm -f *.o y.tab.c y.tab.h lex.yy.c a.out $(PREFIX)
	#Fichier de sortie temporaire
	rm -f res_c.c

#Generate documentation
doxygen:
	@doxygen Doxyfile