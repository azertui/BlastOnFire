#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../ast.h"
#include "../detect.h"
#include <fcntl.h>
#define VERBOSE
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

static void parsing_operation_test(){
	assert_null(parseString("int main(){int a=2+5*8/1;}",NULL));
}

static void parsing_tab_int_test(){
	assert_null(parseString("int main(){int a[5][8];a[4][7]=6;}",NULL));
}

static void parsing_multilignes_test(){
	assert_null(parseString("int main(){int a;a=2+5;}",NULL));
}

static void parsing_while_test(){
	assert_null(parseString("int main(){while(1){}}",NULL));
}

static void parsing_while_test2(){
	assert_null(parseString("int main(){int a=5; while(a<5){}}",NULL));
}

static void parsing_while_test3(){
	assert_null(parseString("int main(){int a=5; while(a<5){int c;c=5+6;}}",NULL));
}

static void parsing_while_test4(){
	assert_null(parseString("int main(){int a=1; while(a<5)a+=1;}",NULL));
}

static void parsingFAIL_while_test(){
	assert_true(parseString("int main(){while(a);}",NULL));
}
static void parsingFAIL_while_test2(){
	assert_true(parseString("int main(){while(4)",NULL));
}
static void parsingFAIL_while_test3(){
	assert_true(parseString("int main(){while(1){int c = 5;}c=6;}",NULL));
}

static void parsing_for_test1(){
	assert_null(parseString("int main(){for(int j=0;j<5;j++)int i; }",NULL));
}
static void parsing_for_test2(){
	assert_null(parseString("int main(){int j; for(j=0;j<5;j++)int i; }",NULL));
}
static void parsing_for_test3(){
	assert_null(parseString("int main(){int j; for(j=0;j<5;)int i; }",NULL));
}

static void parsingFAIL_for_test1(){
	assert_true(parseString("int main(){for(j=0;j<5;j++)int i;}",NULL));
}
static void parsingFAIL_for_test2(){
	assert_true(parseString("int main(){for(int j=0;j<5;k++)int i;}",NULL));
}
static void parsingFAIL_for_test3(){
	assert_true(parseString("int main(){for(j=0;j++;j<5)int i;}",NULL));
}

static void parsing_return_test1(){
    assert_false(parseString("int main(){return 0;}",NULL));
}

static void parsingFAIL_return_test1(){
    assert_true(parseString("int main(){return while;}",NULL));
}

static void equivalent_snippet_basic(){
    //int sum(int x, int y)
    ast* fun = ast_new_main_fct(ast_new_operation(AST_RET,
                                                  NULL,
                                                  ast_new_operation(AST_OP_PLUS,
                                                  ast_new_id("x", NULL, 0, 0, 1),
                                                  ast_new_id("y", NULL, 0, 0, 1))),
                                NULL, "sum", AST_INT);
    fun->fonction.nb_param = 2;
    fun->fonction.params   = malloc(2 * sizeof(ast*));
    fun->fonction.params[0] = ast_new_id("x", NULL, 0, 0, 1);
    fun->fonction.params[1] = ast_new_id("y", NULL, 0, 0, 1);

    //var = a + b;
    ast* snippet = ast_new_id("var", ast_new_operation(AST_OP_PLUS,
                                                       ast_new_id("a", NULL, 0, 0, 1),
                                                       ast_new_id("b", NULL, 0, 0, 1)),
                              0, 0, 1);

    assert_non_null(equivalent_snippet(snippet, fun));
static void parsing_void_function(){
    assert_false(parseString("void test(){return;}",NULL));
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
		cmocka_unit_test_setup_teardown(parsing_operation_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_tab_int_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_multilignes_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_while_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_while_test2,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_while_test3,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_while_test4,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFAIL_while_test,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFAIL_while_test2,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFAIL_while_test3,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_for_test1,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_for_test2,setup,teardown),
		cmocka_unit_test_setup_teardown(parsing_for_test3,setup,teardown),
		cmocka_unit_test_setup_teardown(parsingFAIL_for_test1,setup,teardown),
        cmocka_unit_test_setup_teardown(parsingFAIL_for_test2,setup,teardown),
        cmocka_unit_test_setup_teardown(parsingFAIL_for_test3,setup,teardown),
        cmocka_unit_test_setup_teardown(parsing_return_test1,setup,teardown),
        cmocka_unit_test_setup_teardown(parsingFAIL_return_test1,setup,teardown),
        cmocka_unit_test_setup_teardown(equivalent_snippet_basic, setup, teardown),
        cmocka_unit_test_setup_teardown(parsing_void_function,setup,teardown),
	};
	return cmocka_run_group_tests(tests, NULL, group_teardown);
}
