#ifndef EBNF_PARSER_H
#define EBNF_PARSER_H

#include "lesyange.h"

#define UNKNOWN -1

#define EPISOLON 0
#define DOLLAR 1

#define SEMICOLON 2
#define DERIVES 3
#define TERMINAL 4
#define NONTERMINAL 5
#define RULE 6
#define IDENTIFIER 7

typedef struct {
	short id;
	char *lexeme;
	int line;
	int col;
} ebnf_token_t;


void parse_ebnf(OPT_CALL);
void print_token(ebnf_token_t tk);

#endif
