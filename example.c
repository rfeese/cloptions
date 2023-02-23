/*
 * Copyright 2022 Roger Feese
 */
#include <stdio.h>
#include <stdlib.h>
#include "src/cloptions.h"

int debug = 0;
void debug_callback(char *strval, int intval, float floatval){
	debug = 1;
}

char foo_strval[32] = {};
int foo_intval = 0;
float foo_floatval = 0.0f;
void foo_callback(char *strval, int intval, float floatval){
	snprintf(foo_strval, 32, "%s", strval);
	foo_intval = intval;
	foo_floatval = floatval;
}

char color[8] = {};
void color_callback(char *strval, int intval, float floatval){
	snprintf(color, 8, "%s", strval);
}

char filedir[32] = {};
void filedir_callback(char *strval, int intval, float floatval){
	snprintf(filedir, 32, "%s", strval);
}

char headerfile[32] = {};
void headerfile_callback(char *strval, int intval, float floatval){
	snprintf(headerfile, 32, "%s", strval);
}

int main(int argc, char* argv[]){
	// simple flag option
	cloptions_add("--debug", "", "Enable debug output.", debug_callback);

	// option with argument
	cloptions_add("--foo", "foovalue", "Foo value.", foo_callback);

	// option with argument choices
	cloptions_add("--color", "red | green | blue", "color value.", color_callback);
	cloptions_add_arg_finder("--color", "red | green | blue", "echo \"red green blue\"");

	// nameless positional argument using builtin _filedir finder
	cloptions_add("", "[filedir]", "file or directory", filedir_callback);
	cloptions_add_arg_finder("", "[filedir]", "_filedir");

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
	printf("filedir: \"%s\"\n", filedir);
	printf("headerfile: \"%s\"\n", headerfile);
	return EXIT_SUCCESS;
}
