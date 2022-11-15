# Simple Command-line Options Library

A small library for adding command-line options to a C program. Includes the option to export a bash-completions script from the compiled program.

## Features

   * Nicely-formatted output for built-in help options (-?, --help).
   * Automatic argument parsing into integer and float values.
   * Bash Tab-Completion (see below).

## Types of command-line options supported

This library takes a simplified view of command-line options, limiting them to options of these types:

   1. Flag-style options (ex: --debug)
   2. Options that take a single argument (ex: --inputfile README.md)
   3. Positional arguments / "nameless" options

## Bash Tab-completion

The library is capable of generating bash-completion (Tab-completion) scripts via a "secret" option "--bash-completion". Additionally, it supports providing custom "finders" for option arguments. Finders allow tab-completion to be more dynamic for arguments (ex:  `find . -type f -name '*.c'`)
