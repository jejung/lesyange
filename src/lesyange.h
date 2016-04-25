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

#ifndef LESYANGE_H
#define LESYANGE_H

#define VERSION "0.1"

#define ERROR_OPENING_FILE 1
#define ERROR_UNEXPECTED_EOF 2
#define ERROR_UNTERMINATED_COMMENT 3
#define ERROR_UNTERMINATED_LITERAL 4
#define ERROR_UNEXPECTED_CHAR 5
#define ERROR_UNEXPECTED_TOKEN 6
#define ERROR_READING_FILE 7
#define ERROR_NOT_ENOUGH_MEMORY 8

#define OPT_CALL options_t opt

#define UNEXPECTED_ERROR(macrocode, ...) \
    do { printf("Error (%s) in %s, line %d:\n", #macrocode, __FILE__, __LINE__);  \
        printf(__VA_ARGS__); \
       exit(macrocode); \
    } while(0);

#define MSK_OPT_HELP 0x01
#define MSK_OPT_VERSION 0x02
#define MSK_OPT_DEBUG 0x04
    
#define DEBUG_LOG(opt, type, ...) if ((opt.flags & MSK_OPT_DEBUG) && (type == '*' || opt.debug_type == '*' || type == opt.debug_type)){ printf(__VA_ARGS__); printf("\n"); }

typedef struct {
	char *ebnf_file;
	unsigned int flags;
    char debug_type;
} options_t;

options_t opt_parse_args(int argc,char *argv[]);
void print_options(OPT_CALL);
void print_usage(void);
void print_version(void);
#endif
