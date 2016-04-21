/*  lesyange - Lexical Syntatic Analyzers generator.
    Copyright (C) 2016  Jean Jung

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lesyange.h"

options_t opt_parse_args(int argc, char* argv[]) 
{
	options_t r;
	r.ebnf_file = NULL;
	r.flags = 0;
	if (argc == 1) 
    {
		r.flags = MSK_OPT_HELP;
		return r;
	}
	int i;
	for (i = 1; i < argc; ++i)  
    {
		char* arg = argv[i];
		if (*arg == '-') 
        {
			arg++;
			if (*arg == 'h') 
            {
				r.flags = r.flags | MSK_OPT_HELP;
				break;
			} else if (*arg == 'v') 
            {
				r.flags = r.flags | MSK_OPT_VERSION;
				break;
			} else if (*arg == 'd') 
            {
				r.flags = r.flags | MSK_OPT_DEBUG;
			}
		} else 
			r.ebnf_file = argv[i];	
	}
	return r;
}

void print_options(OPT_CALL) 
{
	printf("\nlesyange version %s called with options:\n", VERSION);
	printf("\tebnf_file: %s\n", opt.ebnf_file);
	if (opt.flags & MSK_OPT_DEBUG) 
    {
		printf("\twith debug output enabled\n");
	}
    printf("\n");
}

void print_usage(void) 
{
	printf("\nValid usage for lesyange\n");
	printf("\tlesyange [-OPTION1, -OPTION2, ...]  ebnf_file\n"); 
	printf("OPTIONS:\n");
	printf("\t-h: print this help text and exit.\n");
	printf("\t-v: print the lesyange version and exit.\n");
	printf("\t-d[=(*|a|l|s): print debug information into the parse process, possible values are:\n");
    printf("\t\t*: means any information, this is the default option.\n");
    printf("\t\ta: just print informations about the semantic actions being fired.\n");
    printf("\t\tl: just print informations about the lexer module.\n");
    printf("\t\ts: just print informations about the syntatic module.\n");
    printf("\n");
	exit(0);
}

void print_version() 
{
	printf("\nlesyange version %s\n\n", VERSION);
}
