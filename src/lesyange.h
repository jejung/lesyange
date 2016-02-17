#ifndef LESYANGE_H
#define LESYANGE_H

#define VERSION "0.1"

#define ERROR_OPENING_FILE 1
#define ERROR_UNEXPECTED_EOF 2
#define ERROR_UNTERMINATED_COMMENT 3
#define ERROR_UNTERMINATED_LITERAL 4
#define ERROR_UNEXPECTED_CHAR 5
#define ERROR_UNEXPECTED_TOKEN 6

#define OPT_CALL options_t opt
#define INCP(pointer) (*pointer = (*pointer + 1));
#define MACRO_NAME(macro) #macro

#define UNEXPECTED_ERROR(fp, macrocode, ...) \
    do { fclose(fp); \
        printf("Error (%s) in %s, line %d:\n", #macrocode, __FILE__, __LINE__);  \
        printf(__VA_ARGS__); \
       exit(macrocode); \
    } while(0); 

#define DEBUG_LOG(opt, ...) if (opt.d){do{printf(__VA_ARGS__); printf("\n");} while(0);}

typedef struct {
	char *ebnf_file;
	short h;
	short v;
	short d;
} options_t;

options_t parse_args(int argc,char *argv[]);
void print_options(OPT_CALL);
void print_usage(void);
void print_version(void);

#endif
