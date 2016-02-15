#include "lesyange.h"
#include "ebnf_parser.h"

int main(int argc, char* argv[]) {
	OPTIONS_T opt = parse_args(argc, argv);
	if (opt.h)
		print_usage();
	else if (opt.v)
       		print_version();
	else{
		print_options(opt);
		parse_ebnf(opt);
	}
	return 0;
}
