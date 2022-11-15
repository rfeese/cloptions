/*
 * Copyright 2022 Roger Feese
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cloptions.h"

int cloptions_num = 0;
struct s_cloption cloptions[CLOPTIONS_MAX] = {};
#define CLOPTION_ARG_FINDER_STR_MAX 512
char cloptions_arg_finder[CLOPTIONS_MAX][CLOPTION_ARG_FINDER_STR_MAX] = {{}};
#define CLOPTIONS_ERROR_MSG_LEN	128
char cloptions_error_msg[CLOPTIONS_ERROR_MSG_LEN] = {};
int cloptions_unnamed_argval_found[CLOPTIONS_MAX] = {};

int cloptions_add(const char *str, const char *argstr, const char *helpstr, cloption_callback callback){
	// make sure the option hasn't already been added
	for(int i = 0; i < cloptions_num; i++){
		if((str[0] && strncmp(cloptions[i].str, str, CLOPTION_STR_MAX) == 0) 
				|| (argstr[0] && strncmp(cloptions[i].argstr, argstr, CLOPTION_STR_MAX) == 0)){
			snprintf(cloptions_error_msg, CLOPTIONS_ERROR_MSG_LEN, "Option already added: %s", str);
			return 0;
		}
	}
	if(cloptions_num < CLOPTIONS_MAX){
		snprintf(cloptions[cloptions_num].str, CLOPTION_STR_MAX, "%s", str);
		snprintf(cloptions[cloptions_num].argstr, CLOPTION_STR_MAX, "%s", argstr);
		snprintf(cloptions[cloptions_num].helpstr, CLOPTION_HELP_STR_MAX, "%s", helpstr);
		cloptions[cloptions_num].callback = callback;
		cloptions_num++;
		return 1;
	}
	return 0;
}

void cloptions_add_arg_finder(const char *str, const char *argstr, const char *finder){
	// add to finders, if it is new
	int finder_idx = -1;
	for(int i = 0; (i < CLOPTIONS_MAX) && (finder_idx < 0); i++){
		if(strnlen(cloptions_arg_finder[i], CLOPTION_STR_MAX) == 0){
			// nothing there, insert it
			snprintf(cloptions_arg_finder[i], CLOPTION_ARG_FINDER_STR_MAX, "%s", finder);
			finder_idx = i;
		}
		else {
			if(strncmp(cloptions_arg_finder[i], finder, CLOPTION_ARG_FINDER_STR_MAX) == 0){
				// matches existing finder
				finder_idx = i;
			}
		}
	}

	if(finder_idx < 0){
		// no space
		return;
	}

	// find option by str or argstr
	if(strnlen(str, CLOPTION_STR_MAX) > 0){
		for(int i = 0; i < cloptions_num; i++){
			if(strncmp(cloptions[i].str, str, CLOPTION_STR_MAX) == 0){
				cloptions[i].arg_finder_str = cloptions_arg_finder[finder_idx];
			}
		}
	}
	if(strnlen(argstr, CLOPTION_STR_MAX) > 0){
		for(int i = 0; i < cloptions_num; i++){
			if(strncmp(cloptions[i].argstr, argstr, CLOPTION_STR_MAX) == 0){
				cloptions[i].arg_finder_str = cloptions_arg_finder[finder_idx];
			}
		}
	}
}

int cloptions_check(int argc, char *argv[]){
	cloptions_error_msg[0] = '\0';
	// start at 1 assuming that we can throw away argv[0]
	for(int i = 1; i < argc; i++){
		// common options
		if((strncmp(argv[i], "-?", CLOPTION_STR_MAX) == 0) || (strncmp(argv[i], "--help", CLOPTION_STR_MAX) == 0)){
			cloptions_print_help(argv[0]);
			exit(EXIT_SUCCESS);
		}

		if(strncmp(argv[i], "--bash-completion", CLOPTION_STR_MAX) == 0){
			cloptions_generate_bash_completion(argv[0]);
			exit(EXIT_SUCCESS);
		}

		// other options
		// loop for named options
		int option_matched = 0;
		for(int j = 0; (j < cloptions_num) && !option_matched; j++){
			char strargval[CLOPTION_VAL_STR_MAX] = {};
			int intargval = 0;
			float floatargval = 0.0f;

			if(strnlen(cloptions[j].str, CLOPTION_STR_MAX) > 0){
				if((strncmp(argv[i], cloptions[j].str, CLOPTION_STR_MAX) == 0)){
					option_matched = 1;
					// if option takes a parameter, read it
					if(strnlen(cloptions[j].argstr, CLOPTION_STR_MAX) > 0){
						// make sure there is another argv ...
						if(argc < i + 2){
							snprintf(cloptions_error_msg, CLOPTIONS_ERROR_MSG_LEN, "%s%s %s", "Missing argument for option: ", cloptions[j].str, cloptions[j].argstr);
							return 0;
						}

						// get strval
						snprintf(strargval, CLOPTION_VAL_STR_MAX, "%s", argv[i + 1]);

						//get intval
						int charsmatching = 0;
						sscanf(argv[i + 1], "%d%n", &intargval, &charsmatching);
						if(charsmatching == strlen(argv[i + 1])){
							// looks like an int
							// if we are checking argument types, we could do more here
						}

						// floatval
						sscanf(argv[i + 1], "%f%n", &floatargval, &charsmatching);
						if(charsmatching == strlen(argv[i + 1])){
							// looks like a float
							// if we are checking argument types, we could do more here
						}

						// go to next argv
						i++;
					}
					// call option callback
					if(cloptions[j].callback){
						cloptions[j].callback(strargval, intargval, floatargval);
					}
				}
			}
		}
		// loop for unnamed argument-only options
		for(int j = 0; (j < cloptions_num) && !option_matched; j++){
			char strargval[CLOPTION_VAL_STR_MAX] = {};
			int intargval = 0;
			float floatargval = 0.0f;

			if(strnlen(cloptions[j].str, CLOPTION_STR_MAX) == 0){
				// find them, in order
				if(cloptions_unnamed_argval_found[j]){
					continue;
				}
				// just consume the unclaimed argument
				cloptions_unnamed_argval_found[j] = 1;
				option_matched = 1;

				// get strval
				snprintf(strargval, CLOPTION_VAL_STR_MAX, "%s", argv[i]);

				// get intval
				int charsmatching = 0;
				sscanf(argv[i], "%d%n", &intargval, &charsmatching);
				if(charsmatching == strlen(argv[i])){
					// looks like an int
					// if we are checking argument types, we could do more here
				}

				// floatval
				sscanf(argv[i], "%f%n", &floatargval, &charsmatching);
				if(charsmatching == strlen(argv[i])){
					// looks like a float
					// if we are checking argument types, we could do more here
				}

				if(cloptions[j].callback){
					cloptions[j].callback(strargval, intargval, floatargval);
				}
			}
		}
		if(option_matched){
			// go to next option/arg
			continue;
		}
		// option not consumed -- invalid option
		snprintf(cloptions_error_msg, CLOPTIONS_ERROR_MSG_LEN, "%s%s", "Unrecognized option: ", argv[i]);
		return 0;
	}
	return 1;
}
char *cloptions_get_error(){
	return cloptions_error_msg;
}

void cloptions_print_help(char *argv0){
	//find longest option name
	int optlength = 0;
	for(int j = 0; j < cloptions_num; j++){
		if((strnlen(cloptions[j].str, CLOPTION_STR_MAX) + strnlen(cloptions[j].argstr, CLOPTION_STR_MAX) + 1)  > optlength){
			optlength = strnlen(cloptions[j].str, CLOPTION_STR_MAX) + strnlen(cloptions[j].argstr, CLOPTION_STR_MAX) + 1;
		}
	}

	char unnamed_options_str[128] = {};
	for(int j = 0; j < cloptions_num; j++){
		if(strnlen(cloptions[j].str, CLOPTION_STR_MAX) == 0){
			strncat(unnamed_options_str, " ", 128 - 1);
			strncat(unnamed_options_str, cloptions[j].argstr, 128 - 1);
		}
	}
	printf("\nUsage: %s [OPTIONS]%s\n\n", argv0, unnamed_options_str);
	char helpline[128] = {};
	snprintf(helpline, 128, "%s", "-?, --help");
	int myoptlength =  strnlen(helpline, 128);
	for(int k = 0; k < optlength - myoptlength; k++){
		strncat(helpline, " ", 128 - 1);
	}
	printf("  %s\t%s\n", helpline, "Show this help.");
	for(int j = 0; j < cloptions_num; j++){
		if(strnlen(cloptions[j].str, CLOPTION_STR_MAX) > 0){
			snprintf(helpline, 128, "%s %s", cloptions[j].str, cloptions[j].argstr);
			int myoptlength =  strnlen(helpline, 128);
			for(int k = 0; k < optlength - myoptlength; k++){
				strncat(helpline, " ", 128 - 1);
			}
			printf("  %s\t%s\n", helpline, cloptions[j].helpstr);
		}
	}
	printf("\n");
}

void cloptions_generate_bash_completion(char *argv0){
	char myname[64] = {};
	snprintf(myname, 64, "%s", argv0);

	// clean up myname
	for(int i = 0; (i < CLOPTION_STR_MAX) && myname[i]; i++){
		if(
			   (myname[i] != 'a') && (myname[i] != 'b') && (myname[i] != 'c') && (myname[i] != 'd')
			&& (myname[i] != 'e') && (myname[i] != 'f') && (myname[i] != 'g') && (myname[i] != 'h')
			&& (myname[i] != 'i') && (myname[i] != 'j') && (myname[i] != 'k') && (myname[i] != 'l')
			&& (myname[i] != 'm') && (myname[i] != 'n') && (myname[i] != 'o') && (myname[i] != 'p')
			&& (myname[i] != 'q') && (myname[i] != 'r') && (myname[i] != 's') && (myname[i] != 't')
			&& (myname[i] != 'u') && (myname[i] != 'v') && (myname[i] != 'w') && (myname[i] != 'x')
			&& (myname[i] != 'y') && (myname[i] != 'z') && (myname[i] != '_')
		)
		{
			myname[i] = 'x';
		}
	}

	// functions to help find values for option args
	for(int i = 0; i < CLOPTIONS_MAX; i++){
		// don't create finders for builtins (eg _filedir), assume they start with underscore
		if(strnlen(cloptions_arg_finder[i], CLOPTION_ARG_FINDER_STR_MAX) > 0
				&& cloptions_arg_finder[i][0] != '_'){
			printf("_%s_finder_%d(){\n", myname, i);
			printf("\tlocal foundstuff\n");
			printf("\tfoundstuff=$( %s )\n", cloptions_arg_finder[i]);
			printf("\tCOMPREPLY=( $(compgen -W \"$foundstuff\" -- \"$cur\" ) )\n");
			printf("}\n");
		}
	}

	// function to check options that require arguments
	// start check_args
	printf("_%s_opt_args(){\n", myname);
	printf("\tcase $prev in\n");
	for(int i = 0; i < cloptions_num; i++){
		// options that take an argument
		if((strnlen(cloptions[i].str, CLOPTION_STR_MAX) > 0) && (cloptions[i].str[0] == '-') && (strnlen(cloptions[i].argstr, CLOPTION_STR_MAX) > 0)){
			printf("\t\t%s)\n", cloptions[i].str);
			if(cloptions[i].arg_finder_str){
				// call builtins directly (eg _filedir), assume they start with underscore
				if(cloptions[i].arg_finder_str[0] == '_'){
					printf("\t\t\t%s\n", cloptions_arg_finder[i]);
				}
				else{
					int finder_num = -1;
					for(int j = 0; j < CLOPTIONS_MAX; j++){
						if(cloptions[i].arg_finder_str == cloptions_arg_finder[j]){
							finder_num = j;
						}
					}
					if(finder_num >= 0){
						printf("\t\t\t_%s_finder_%d\n", myname, finder_num);
					}
				}
				printf("\t\t\treturn\n");
			}
			else{
				printf("\t\t\treturn 0\n");
			}
			printf("\t\t\t;;\n");
		}
	}
	printf("\tesac\n");
	printf("\treturn 1\n");
	// end check_args
	printf("}\n");
	
	
	// regular options for completion
	char options_str[512] = " -? --help";
	// options that take arguments glob
	char options_with_args_glob[512] = "@(";
	for(int i = 0; i < cloptions_num; i++){
		// normal options start with a dash
		if((strnlen(cloptions[i].str, CLOPTION_STR_MAX) > 0) && (cloptions[i].str[0] == '-')){
			strncat(options_str, " ", 512 - 1);
			strncat(options_str, cloptions[i].str, 512 - 1);
			if(cloptions[i].argstr){
				strncat(options_with_args_glob, cloptions[i].str, 512 - 1);
				strncat(options_with_args_glob, "|", 512 - 1);
			}
		}
	}
	strncat(options_with_args_glob, ")", 512 - 1);

	// custom arg counter that ignores options that take arguments
	printf("_%s_count_args(){\n", myname);
	printf("\targs=1\n");
	printf("\tlocal offset=1\n");
	printf("\tlocal i prev=${words[$offset-1]}\n");
	printf("\tfor (( i=$offset; i < cword; i++ )); do\n");
	printf("\t\tif [[ \"${words[i]}\" != -* && $prev != %s ]]; then\n", options_with_args_glob);
	printf("\t\t\targs=$(($args + 1))\n");
	printf("\t\tfi\n");
	printf("\t\tprev=${words[i]}\n");
	printf("\tdone\n");
	printf("}\n");

	// start main
	printf("_%s_complete(){\n", myname);
	printf("\tlocal cur prev words cword\n");
	printf("\t_init_completion || return\n");
	printf("\tif [[ $cur == -* ]]; then\n");
	printf("\t\tCOMPREPLY=( $( compgen -W \"%s\" -- \"$cur\" ) )\n", options_str);
	printf("\t\treturn\n");
	printf("\tfi\n");

	// call check option args
	printf("\t_%s_opt_args && return\n", myname);

	// unnamed options
	printf("\t_%s_count_args\n", myname);
	int unnamed_opt_num = 0;
	for(int i = 0; i < cloptions_num; i++){
		if((strnlen(cloptions[i].str, CLOPTION_STR_MAX) == 0) && (strnlen(cloptions[i].argstr, CLOPTION_STR_MAX) > 0)){
			unnamed_opt_num++;
		}
		else{
			continue;
		}

		if(cloptions[i].arg_finder_str){
			int finder_num = -1;
			for(int j = 0; j < CLOPTIONS_MAX; j++){
				if(cloptions[i].arg_finder_str == cloptions_arg_finder[j]){
					finder_num = j;
				}
			}
			if(finder_num >= 0){
				printf("\tif [[ $args -eq %d ]]; then\n", unnamed_opt_num);
				// call builtins directly (eg _filedir), assume they start with underscore
				if(cloptions[i].arg_finder_str[0] == '_'){
					printf("\t\t%s\n", cloptions[i].arg_finder_str);
				}
				else {
					printf("\t\t_%s_finder_%d\n", myname, finder_num);
				}
				printf("\t\treturn\n");
				printf("\tfi\n");
			}
		}
	}
	
	// end main
	printf("} &&\n");
	printf("complete -F _%s_complete %s\n", myname, argv0);
}
