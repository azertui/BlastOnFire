TESTDIR = tests

all : README.md $(TESTDIR) $(TESTDIR)/tests

README.md : ;

$(TESTDIR)/tests : $(TESTDIR)/tests.o
	gcc -o $(TESTDIR)/tests $(TESTDIR)/tests.o -l cmocka -L /usr/local/lib
$(TESTDIR) : 
	mkdir -p $(TESTDIR)

$(TESTDIR)/tests.o : $(TESTDIR)/tests.c
	gcc -c $(TESTDIR)/tests.c -o $(TESTDIR)/tests.o -l cmocka -L /usr/local/lib

.PHONY: test clean
test :
	./$(TESTDIR)/tests

clean :
	rm -f $(TESTDIR)/*.o