#ifndef EBNF_PARSER_H
#define EBNF_PARSER_H

#include "lesyange.h"

#define UNKNOWN -1 // ~?~

#define EPISOLON 0 // ÿ
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
#define OPEN_COMMENT 15 // (*
#define CLOSE_COMMENT 16 // *)
#define SPECIAL_SEQ 17 // ?
#define EXCEPTION 18 // -
#define TERMINATION_DOT 19 // .
#define TK_EOF 20 // EOF

#define TK_SETID(tk, value) do {tk.id = value; tk.class = #value;} while(0);
#define TK_SETLEX(tk, c) do {tk.lexeme = malloc(sizeof(char) * 2); sprintf(tk.lexeme, "%c", c);} while(0);

typedef struct {
	short id;
    const char* class;
	char *lexeme;
	int line;
	int col;
} ebnf_token_t;


void parse_ebnf(OPT_CALL);
void print_token(ebnf_token_t tk);

#endif
