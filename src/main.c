/*  lesyange - Lexical and Syntatic Analyzers Generator.
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

#include "lesyange.h"
#include "cextensions.h"
#include "ebnf_parser.h"

int main(int argc, char* argv[]) 
{
	options_t opt = opt_parse_args(argc, argv);
	if (opt.flags & MSK_OPT_HELP)
		print_usage();
	else if (opt.flags & MSK_OPT_VERSION)
       	print_version();
    else
    {
		print_options(opt);
		parse_ebnf(opt);
        printf("EBNF file ok.");
	}
	return 0;
}
