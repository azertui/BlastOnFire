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

#ifndef VERBOSE
//used to redirect stdout
int temp;
static int setup(){
	temp=dup(stdout->_fileno);
	freopen("tests/test_logs.txt","w",stdout);
	return 0;
}
static int teardown(){
	//int fd = open("/dev/tty", O_WRONLY);

    stdout = fdopen(temp, "w");
	return 0;
}

static int group_teardown(){
	if(remove("tests/test_logs.txt")!=0){
		fprintf(stderr,"group_teardown:error while removing logs\n");
		return 1;
	}
	return 0;
}
#else
static int setup(){return 0;}
static int teardown(){return 0;}
static int group_teardown(){return 0;}
#endif
extern int parseString(char*,ast*);
extern int parseFile(FILE*, ast*);

static void parsing_basicExemple_test(){
	assert_null(parseString("int main(){int i;}\n",NULL));
}
static void reentrance_test(){
	assert_null(parseString("int main(){int i;}\n",NULL));
}
static void parsingFail_function_test(){
	assert_true(parseString("int main(\n",NULL)); //autre sortie que 0 => true en C
}
static void parsingFile_test(){
	FILE* f=fopen("tests/code_c.c","r");
	if(f==NULL){
		print_error("File code_c.c doesn't exist anymore\n");
		fail();
	}
	int res = parseFile(f,NULL);
	fclose(f);
	assert_null(res);
}
static void parsingFail_undeclared_test(){
	assert_true(parseString("int main(){a=2;}",NULL));
}
static void ast_test(){
	ast a;
	if(parseString("int main(){}",&a)!=0)
		fail();
	assert_int_equal(a.type,AST_FCT);
}

static void parsing_addition_simple_test(){
	assert_null(parseString("int main(){int a=2+5;}",NULL));
}

static void parsing_soustraction_simple_test(){
	assert_null(parseString("int main(){int a=2-5;}",NULL));
}

static void parsing_multiplication_simple_test(){
	assert_null(parseString("int main(){int a=2*5;}",NULL));
}

static void parsing_division_simple_test(){
	assert_null(parseString("int main(){int a=2/5;}",NULL));
}

static void parsing_multilignes_test(){
	assert_null(parseString("int main(){int a;a=2+5;}",NULL));
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_setup_teardown(parsing_basicExemple_test,setup,teardown),
		cmocka_unit_test_setup_teardown(reentrance_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFail_function_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFile_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFail_undeclared_test,setup,teardown),
		cmocka_unit_test_setup_teardown(ast_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_addition_simple_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_soustraction_simple_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_multiplication_simple_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_division_simple_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_multilignes_test,setup,teardown),
	};
	return cmocka_run_group_tests(tests, NULL, group_teardown);
}