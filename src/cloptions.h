/*
 * Copyright 2022 Roger Feese
 */
#ifndef CLOPTIONS_H
#define CLOPTIONS_H

#define CLOPTION_STR_MAX	32
#define CLOPTION_HELP_STR_MAX	128
#define CLOPTION_VAL_STR_MAX	64
typedef void (*cloption_callback)(char *strval, int intval, float floatval);
struct s_cloption {
	char str[CLOPTION_STR_MAX];       // --str
	char argstr[CLOPTION_STR_MAX];  // --str [argstr]
	char helpstr[CLOPTION_HELP_STR_MAX];
	cloption_callback callback;
	char *arg_finder_str;
};

#define CLOPTIONS_MAX	24
extern int cloptions_num;
extern struct s_cloption cloptions[CLOPTIONS_MAX];

int cloptions_add(const char *str, const char *argstr, const char *helpstr, cloption_callback callback);
void cloptions_add_arg_finder(const char *str, const char *argstr, const char *finder);
int cloptions_check(int argc, char *argv[]); // check common and any additional command-line options
char *cloptions_get_error();
void cloptions_print_help(char *argv0);
void cloptions_generate_bash_completion(char *argv0);
#endif //CLOPTIONS_H
