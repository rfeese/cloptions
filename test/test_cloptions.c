#include <string.h>
#include <stdlib.h>
#include "../../Unity/src/unity.h"
#include "../src/cloptions.h"

// substitute
int printf_called = 0;
int printf (const char *__restrict __format, ...){
	printf_called = 1;
	return 1;
}

// runs before each test
void setUp(void){
	printf_called = 0;
}

//runs after each test
void tearDown(void){
}

char argstrval[32] = {};
int argintval = 0;
float argfloatval = 0.0f;
int option1_callback_called = 0;
void option1_callback(char *strval, int intval, float floatval){
	option1_callback_called = 1;
	snprintf(argstrval, 32, "%s", strval);
	argintval = intval;
	argfloatval = floatval;
}

void test_cloptions_add(){
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("--option1", "option1 arg", "option1 help", option1_callback), "adding option should succeed.");
}

void test_cloptions_check(){
	int argc = 6;
	char *argv[] = { "test", "--option1", "8.1", "--nameless", "--", "-notanoption" };
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv), "should NOT have checked options successfully -- undefined option.");
}

void test_cloptions_get_error(){
	int argc = 1;
	char *argv[] = { "test", "--invalidoption" };

	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_check(argc, argv), "should have checked options successfully.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, strncmp("", cloptions_get_error(), 1), "Should not be any error message.");
}

void test_cloptions_print_help(){
	char *argv[] = { "test" };
	cloptions_print_help(argv[0]);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, printf_called, "printf should have been called.");
}

void test_cloptions_generate_bash_completion(){
	char *argv[] = { "test" };
	cloptions_generate_bash_completion(argv[0]);
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, printf_called, "printf should have been called.");
}

int main(){
	UNITY_BEGIN();
	RUN_TEST(test_cloptions_add);
	RUN_TEST(test_cloptions_check);
	RUN_TEST(test_cloptions_get_error);
	RUN_TEST(test_cloptions_print_help);
	RUN_TEST(test_cloptions_generate_bash_completion);

	return UNITY_END();
}
