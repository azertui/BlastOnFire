TESTDIR = tests
PREFIX = parser
CFLAGS =
yFLAGS =
ifdef DEBUG
CFLAGS = -Wall -ggdb3
yFLAGS = -v
endif

#All
all : README.md $(PREFIX) $(TESTDIR) $(TESTDIR)/tests

#Executable principal (nom : variable PREFIX)
$(PREFIX): main.o y.tab.o lex.yy.o ast.o symboles.o
	gcc $^ $(CFLAGS) -ly -lfl -o $@

#Fichier objet YACC
y.tab.o: $(PREFIX).y lex.h
	yacc $(yFLAGS) -t -d $(PREFIX).y
	gcc -c $(CFLAGS) y.tab.c

#Fichier objet LEX
lex.yy.o: $(PREFIX).l y.tab.h
	gcc -c $(CFLAGS) lex.yy.c

#Fichiers objets classiques
%.o: %.c
	gcc -c $(CFLAGS) $*.c

lex.h: parser.l
	flex parser.l

#Executable de test
$(TESTDIR)/tests : $(TESTDIR)/tests.o y.tab.o lex.yy.o ast.o symboles.o detect.o
	gcc -o $@ $(CFLAGS) $^ -ly -lfl -l cmocka -L /usr/local/lib

#Dossier de test
$(TESTDIR) : 
	mkdir -p $(TESTDIR)

#Fichiers objets de test
$(TESTDIR)/%.o : $(TESTDIR)/%.c $(TESTDIR)
	gcc -c $< $(CFLAGS) -o $@ -l cmocka -L /usr/local/lib

#Phony
.PHONY: test clean doxygen

#Cible de test
test : $(TESTDIR)/tests $(PREFIX)
	./$(TESTDIR)/tests 2>/dev/null
	./testing.sh
#Clean
clean :
	rm -f $(TESTDIR)/*.o $(TESTDIR)/tests
	rm -f *.o y.tab.c y.tab.h lex.yy.c a.out $(PREFIX) lex.h
	#Fichier de sortie temporaire
	rm -f res_c.c
	rm -rf Doxygen/

#Generate documentation
doxygen:
	@doxygen Doxyfile
