#ifndef EBNF_PARSER_H
#define EBNF_PARSER_H

#include "lesyange.h"

#define UNKNOWN -1 // ?

#define START 0 // S
#define DOLLAR 1 // $
#define TERMINATION_SC 2 // ;
#define DEFINE 3 // =
#define IDENTIFIER 4 // foo or <foo>
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
#define EXCEPTION 16 // -
#define TERMINATION_DOT 17 // .

// Non-terminals
#define NT_LHS 18
#define NT_RHS 19
#define NT_RULE 20
#define NT_GRAMMAR 21
#define NT_FINAL 22

#define TK_SETID(tk, value) do {tk.id = value; tk.class = #value;} while(0);
#define TK_SETLEX(tk, c) do {free(tk.lexeme); tk.lexeme = malloc(sizeof(char) * 2); sprintf(tk.lexeme, "%c", c);} while(0);

#define FIRST_NT NT_LHS

#define LL_TABLE \
{\
    {-1,-1,00,}, /*NT_RULE*/\
    {00,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}/*NT_GRAMMAR*/\
}

#define PRODUCTIONS \
{ \
    {NT_RULE},                                  /*0. grammar = { rule };*/\
    {NT_LHS, DEFINE, NT_RHS, TERMINATION_SC},   /*1. rule = lhs, "=", rhs, ";" ;*/\
    {IDENTIFIER},                               /*2. rhs = identifier;*/\
    {TERMINAL_DQ},                              /*3. rhs = terminal_dq;*/\
    {TERMINAL_SQ},                              /*4. rhs = terminal_sq;*/\
    {OPEN_OPTION, NT_RHS, CLOSE_OPTION},        /*5. rhs = "[" rhs "]";*/\
    {OPEN_REPETITION, NT_RHS, CLOSE_REPETITION},/*6. rhs = "{" rhs "}";*/\
    {OPEN_GROUP, NT_RHS, CLOSE_GROUP},          /*7. rhs = "(" rhs ")";*/\
    {NT_RHS, UNION, NT_RHS},                    /*8. rhs = rhs "|" rhs;*/\
    {NT_RHS, CAT, NT_RHS},                      /*9. rhs = rhs "," rhs;*/\
    {IDENTIFIER},                               /*10. lhs = identifier;*/\
    {DOLLAR}                                    /*11. empty*/\
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
