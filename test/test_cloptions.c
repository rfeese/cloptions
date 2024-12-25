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
	cloptions_reset();
}

char argstrval[32] = {};
int argintval = 0;
float argfloatval = 0.0f;
int option1_callback_called = 0;
int option1_callback(char *strval, int intval, float floatval){
	option1_callback_called = 1;
	snprintf(argstrval, 32, "%s", strval);
	argintval = intval;
	argfloatval = floatval;
	return 1;
}

int option2_callback_fails_called = 0;
int option2_callback_fails(char *strval, int intval, float floatval){
	option2_callback_fails_called = 1;
	snprintf(argstrval, 32, "%s", strval);
	argintval = intval;
	argfloatval = floatval;
	return 0;
}

int arg1_callback_called = 0;
int arg1_callback(char *strval, int intval, float floatval){
	arg1_callback_called = 1;
	snprintf(argstrval, 32, "%s", strval);
	argintval = intval;
	argfloatval = floatval;
	return 1;
}

void test_cloptions_add(){
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("--option1", "option1 arg", "option1 help", option1_callback), "adding option should succeed.");
}

void test_cloptions_check(){
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("--option1", "option1 arg", "option1 help", option1_callback), "adding option should succeed.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("--option2", "option2 arg", "option2 help", option2_callback_fails), "adding option should succeed.");

	char *argv[] = { "test", "--option1", "8.1", "--option2", "1" };
	int argc = 2;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv), "should NOT have checked options successfully -- option1 missing argval.");
	argc = 3;
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_check(argc, argv), "should have checked options successfully.");
	argc = 5;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv), "should NOT have checked options successfully -- option2 callback fails.");
	char *argv2[] = { "test", "--option1", "1", "--nameless" };
	argc = 4;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv2), "should NOT have checked options successfully -- undefined option used.");
	char *argv3[] = { "test", "--option1", "1", "--", "-notanoption", "extraarg" };
	argc = 5;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv3), "should NOT have checked options successfully -- extra argument after -- .");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("", "arg1", "arg1 help", option1_callback), "adding option should succeed.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_check(argc, argv3), "should have checked options successfully -- argument after --.");
	argc = 6;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv3), "should NOT have checked options successfully -- extra argument.");
}

void test_cloptions_get_error(){
	int argc = 1;
	char *argv[] = { "test", "--invalidoption" };

	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_check(argc, argv), "should have checked options successfully.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, strncmp("", cloptions_get_error(), 1), "Should not be any error message.");
}

void test_cloptions_set_error(){
	cloptions_set_error("test_set_error");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, strncmp("test_set_error", cloptions_get_error(), 15), "Error message should have been set.");
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
	RUN_TEST(test_cloptions_set_error);
	RUN_TEST(test_cloptions_print_help);
	RUN_TEST(test_cloptions_generate_bash_completion);

	return UNITY_END();
}
