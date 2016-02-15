#ifndef LESYANGE_H
#define LESYANGE_H

#define VERSION "0.1"
#define ERROR_OPENING_FILE 1
#define OPT_CALL options_t opt
#define INCP(pointer) *pointer = *pointer + 1;
#define MACRO_NAME(macro) #macro

typedef struct {
	char *ebnf_file;
	short h;
	short v;
	short d;
} options_t;

typedef long long filepos_t;

options_t parse_args(int argc,char *argv[]);
void print_options(OPT_CALL);
void print_usage(void);
void print_version(void);

#endif
