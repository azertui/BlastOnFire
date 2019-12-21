#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../ast.h"
#include <fcntl.h>

extern int parseString(char*);
extern int parseFile();

static void parsing_basicExemple_test(){
	assert_null(parseString("int main(){int i;}\n"));
}
static void parsingFail_function_test(){
	assert_true(parseString("int main(\n")); //autre sortie que 0 => true en C
}
static void parsingFile_test(){
	FILE* f=fopen("tests/code_c.c","r");
	if(f==NULL){
		print_error("File code_c.c doesn't exist anymore\n");
		fail();
	}
	int res = parseFile(f);
	fclose(f);
	assert_null(res);
}
static void parsingFail_undeclared_test(){
	assert_true(parseString("int main(){a=2;}"));
}

//used to redirect stdout
int temp;

static int setup(){
	freopen("/dev/null","w",stdout);
	return 0;
}

static int teardown(){
	int fd = open("/dev/tty", O_WRONLY);
    stdout = fdopen(fd, "w");
	return 0;
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(parsing_basicExemple_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFail_function_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFile_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFail_undeclared_test,setup,teardown),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}