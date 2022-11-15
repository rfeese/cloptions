#include <string.h>
#include <stdlib.h>
#include "../../Unity/src/unity.h"
#include "../src/cloptions.h"

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

int option2_callback_called = 0;
void option2_callback(char *strval, int intval, float floatval){
	option2_callback_called = 1;
}

int option3_callback_called = 0;
void option3_callback(char *strval, int intval, float floatval){
	option3_callback_called = 1;
}


// runs before each test
void setUp(void){
	argstrval[0] = '\0';
	argintval = 0;
	argfloatval = 0.0f;
	option1_callback_called = 0;
	option2_callback_called = 0;
	option3_callback_called = 0;
	cloptions_num = 0;
}

//runs after each test
void tearDown(void){
}

void test_cloptions_add(){
	// add regular options
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_num, "number of options should default to zero.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("--option1", "option1 arg", "option1 help", option1_callback), "adding option should succeed.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_num, "number of options should be 1.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, strncmp("--option1", cloptions[0].str, CLOPTION_STR_MAX), "option str should have been copied.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, strncmp("option1 arg", cloptions[0].argstr, CLOPTION_STR_MAX), "option argstr should have been copied.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, strncmp("option1 help", cloptions[0].helpstr, CLOPTION_STR_MAX), "option helpstr should have been copied.");
	TEST_ASSERT_EQUAL_PTR_MESSAGE(option1_callback, cloptions[0].callback, "option callback should have been set.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_add("--option1", "option1 arg", "option1 help", option1_callback), "adding duplicate option should fail.");
 
	// add nameless options
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("", "[namelessval]", "just a value", option1_callback), "adding nameless option should succeed.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(2, cloptions_num, "number of options should be 2.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("", "[namelessval2]", "just a value2", option1_callback), "adding second nameless option should succeed.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(3, cloptions_num, "number of options should be 3.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_add("", "[namelessval]", "just a value2", option1_callback), "adding duplicate nameless option should fail.");

	cloptions_num = CLOPTIONS_MAX;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_add("--optionX", "optionX arg", "optionX help", option1_callback), "adding option should fail.");
}

void test_cloptions_check(){
	int argc = 0;
	char *argv[] = { "test", "--option1", "8.1", "--option2" };

	argc = 2;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv), "should NOT have checked options successfully.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("--option1", "[option1 arg]", "option1 help", option1_callback), "adding option should succeed.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv), "should NOT have checked options successfully.");

	argc = 3;
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_check(argc, argv), "should have checked options successfully.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, option1_callback_called, "option1_callback should have been called.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(8, argintval, "option1 argument int val should have been 8.");
	TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01f, 8.1f, argfloatval, "option1 argument float val should have been 8.1");

	// add nameless option	
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_add("", "[namelessval]", "just a value", option2_callback), "adding nameless option should succeed.");
	argc = 4;
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_check(argc, argv), "should have checked options successfully.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, option2_callback_called, "option2_callback should have been called.");
	}

void test_cloptions_get_error(){
	int argc = 1;
	char *argv[] = { "test", "--invalidoption" };
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, cloptions_check(argc, argv), "should have checked options successfully.");
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, strncmp("", cloptions_get_error(), 1), "Should not be any error message.");

	// send invalid option
	argc = 2;
	TEST_ASSERT_EQUAL_INT_MESSAGE(0, cloptions_check(argc, argv), "should NOT have checked options successfully.");
	TEST_ASSERT_LESS_THAN_INT_MESSAGE(0, strncmp("", cloptions_get_error(), 1), "Should have an error message.");
}

void test_cloptions_print_help(){
	// nothing to test
}

void test_cloptions_generate_bash_completion(){
	// nothing to test
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
