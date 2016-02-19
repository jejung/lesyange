#include "lesyange.h"
#include "cextensions.h"
#include "ebnf_parser.h"

int main(int argc, char* argv[]) 
{
	options_t opt = parse_args(argc, argv);
	if (opt.h)
		print_usage();
	else if (opt.v)
       	print_version();
    else if (opt.c) 
        cat_file(opt.ebnf_file);
	else
    {
		print_options(opt);
		parse_ebnf(opt);
	}
	return 0;
}
