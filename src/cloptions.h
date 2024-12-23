/*
 * Copyright 2022 Roger Feese
 */
#ifndef CLOPTIONS_H
#define CLOPTIONS_H
/**
 * Prototype for an option callback.
 * Accepts values for different argument types, even though only one may actually be used.
 */
typedef void (*cloption_callback)(char *strval, int intval, float floatval);

/**
 * Define a command-line option or argument. Adds a "flag" type option, option with argument, 
 * or positional argument depending on the values provided for the first two parameters.
 * Options will be handled first, followed by arguments in the order added.
 *
 * Flag-type option:		("--myflag", "", "Enable myflag.", myflag_callback)
 * Option with argument:	("--myoption", "myoptarg", "Do myoption with arg.", myoption_callback)
 * Positional argument:		("", "myarg", "Use arg.", myarg_callback)
 *
 * \param str Option string (e.g. "--option", "-o")
 * \param argstr Argument string: The name for a user-provided value.
 * \param help Help string.
 * \param callback Callback function pointer.
 * \return 1 if option was added successfully.
 */
int cloptions_add(const char *str, const char *argstr, const char *helpstr, cloption_callback callback);

/**
 * Add a shell command that can be used for tab-expansion on the option argument string.
 * The shell command should evaluate to a list of words separated by spaces.
 *
 * Example - a list of words: 				"echo \" red green blue \""
 * Example - files or directories: 			"_filedir"
 * Example - directories only: 				"_filedir -d"
 * Example - files matching extension (*.c, *.h): 	"_filedir '@(c|h)'"
 *
 * \param str Option string that identifies a previously-defined option.
 * \param argstr Argument string that identifies a previously-defined option arument.
 * \param finder Shell command string used to get possible values for shell tab expansion.
 */
void cloptions_add_arg_finder(const char *str, const char *argstr, const char *finder);

/**
 * Verify and process program arugments per the defined options.
 * As each option is checked, callback functions are called.
 *
 * \param argc Argument count from main()
 * \param argv Argument array from main()
 * \return 1 if user arguments are compliant.
 */
int cloptions_check(int argc, char *argv[]); // check common and any additional command-line options

/**
 * Get message for the most recent error
 *
 * \return Error string.
 */
char *cloptions_get_error();

/**
 * Print a help/usage text based on the options configuration on STDOUT.
 *
 * \param argv0 The name of the executable
 */
void cloptions_print_help(char *argv0);

/**
 * Print a bash completion script based on the defined options and finders on STDOUT.
 *
 * \param argv0 The name of the executable
 */
void cloptions_generate_bash_completion(char *argv0);
#endif //CLOPTIONS_H
