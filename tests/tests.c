#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../ast.h"
#include "../y.tab.h"

int sum(int a,int b){
    return a + b;
}

static void parsing_test(){
	char* str=malloc(50);
	sprintf(str,"int main(){int i;}\n");
	assert_null(parseString(str));
}
static void parsingFail_test(){
	char* str=malloc(50);
    ast** stub;
	sprintf(str,"int main(\n");
    assert_true(parseString(str, stub)); //autre sortie que 0 => true en C
}

static void failing_test() {
	assert_false(1 == 2);
}

static void sum_test(){
    assert_int_equal(5,sum(4,1));
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(failing_test),
        cmocka_unit_test(sum_test),
		cmocka_unit_test(parsing_test),
		cmocka_unit_test(parsingFail_test),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
