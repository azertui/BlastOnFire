#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

int sum(int a,int b){
    return a + b;
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
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}