/*
 * Copyright 2024 Roger Feese
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/cloptions.h"

int debug = 0;
int debug_callback(char *strval, int intval, float floatval){
	debug = 1;
	return 1;
}

char foo_strval[32] = {};
int foo_intval = 0;
float foo_floatval = 0.0f;
int foo_callback(char *strval, int intval, float floatval){
	snprintf(foo_strval, 32, "%s", strval);
	foo_intval = intval;
	foo_floatval = floatval;
	return 1;
}

char color[8] = {};
int color_callback(char *strval, int intval, float floatval){
	snprintf(color, 8, "%s", strval);
	if(!((strncmp(color, "red", 8) == 0)
		|| (strncmp(color, "blue", 8) == 0)
		|| (strncmp(color, "green", 8) == 0))){
		return 0;
	}
	return 1;
}

char myfile[32] = {};
int myfile_callback(char *strval, int intval, float floatval){
	snprintf(myfile, 32, "%s", strval);

	// check the file
	FILE *testfile = fopen(myfile, "r");
	if(!testfile){
		// provide a specific error message.
		cloptions_set_error("The specified file cannot be opened.");
		return 0;
	}
	else{
		fclose(testfile);
	}
	return 1;
}

char headerfile[32] = {};
int headerfile_callback(char *strval, int intval, float floatval){
	snprintf(headerfile, 32, "%s", strval);
	return 1;
}

int main(int argc, char* argv[]){
	// simple flag option
	cloptions_add("--debug", "", "Enable debug output.", debug_callback);

	// option with argument
	cloptions_add("--foo", "foovalue", "Foo value.", foo_callback);

	// option with argument choices using custom finder
	cloptions_add("--color", "( red | green | blue )", "color value.", color_callback);
	cloptions_add_arg_finder("--color", "( red | green | blue )", "echo \"red green blue\"");

	// nameless positional argument using builtin _filedir finder
	cloptions_add("", "[file]", "File to process.", myfile_callback);
	cloptions_add_arg_finder("", "[file]", "_filedir");

	// nameless positional argument using custom finder
	cloptions_add("", "[headerfile]", "Header file", headerfile_callback);
	cloptions_add_arg_finder("", "[headerfile]", "find src/ -type f -name '*.h'");

	if(!cloptions_check(argc, argv)){
		printf("ERROR: %s\n", cloptions_get_error());
		exit(EXIT_FAILURE);
	}

	if(debug){
		printf("DEBUG: Options checked successfully.\n");
	}
	
	printf("\n");
	printf("foo values: (string: \"%s\"; int: %d; float: %f;)\n", foo_strval, foo_intval, foo_floatval);
	printf("color: \"%s\"\n", color);
	printf("file: \"%s\"\n", myfile);
	printf("headerfile: \"%s\"\n", headerfile);
	return EXIT_SUCCESS;
}
