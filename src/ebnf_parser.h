#ifndef EBNF_PARSER_H
#define EBNF_PARSER_H

#include "lesyange.h"

#define UNKNOWN -1 // ?

#define START 0 // S
#define DOLLAR 1 // $
#define TERMINATION_SC 2 // ;
#define DEFINE 3 // =
#define IDENTIFIER 4 // foo
#define CAT 5 // ,
#define UNION 6 // |
#define OPEN_OPTION 7 // [
#define CLOSE_OPTION 8 // ]
#define OPEN_REPETITION 9 // {
#define CLOSE_REPETITION 10 // }
#define OPEN_GROUP 11 // (
#define CLOSE_GROUP 12 // )
#define TERMINAL_DQ 13 // "
#define TERMINAL_SQ 14 // '
#define COMMENT 15 // (**)
#define OPEN_SPECIAL_SEQ 16 // <
#define CLOSE_SPECIAL_SEQ 17 // >
#define EXCEPTION 18 // -
#define TERMINATION_DOT 19 // .

#define TK_SETID(tk, value) do {tk.id = value; tk.class = #value;} while(0);
#define TK_SETLEX(tk, c) do {tk.lexeme = malloc(sizeof(char) * 2); sprintf(tk.lexeme, "%c", c);} while(0);

#define NT_LHS 0
#define NT_RHS 1
#define NT_RULE 2
#define NT_GRAMMAR 3
#define NT_FINAL 4

#define FIRST_NT NT_GRAMMAR

#define LL_TABLE \
    {   \
        /*0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19*/\
        
        {}
        {NT_RULE,NT_FINAL,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} /*NT_GRAMMAR*/ \
    }
    
typedef struct {
	short id;
    const char* class;
	char *lexeme;
	int line;
	int col;
} ebnf_token_t;

void parse_ebnf(OPT_CALL);

#endif
