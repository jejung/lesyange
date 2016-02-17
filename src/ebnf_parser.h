#ifndef EBNF_PARSER_H
#define EBNF_PARSER_H

#include "lesyange.h"

#define UNKNOWN -1 // ?

#define DOLLAR 0 // $
#define TERMINATION_SC 1 // ;
#define DEFINE 2 // =
#define IDENTIFIER 3 // foo or <foo>
#define CAT 4 // ,
#define UNION 5 // |
#define OPEN_OPTION 6 // [
#define CLOSE_OPTION 7 // ]
#define OPEN_REPETITION 8 // {
#define CLOSE_REPETITION 9 // }
#define OPEN_GROUP 10 // (
#define CLOSE_GROUP 11 // )
#define TERMINAL_DQ 12 // "
#define TERMINAL_SQ 13 // '
#define COMMENT 14 // (**)
#define EXCEPTION 15 // -
#define TERMINATION_DOT 16 // .

// Non-terminals
#define NT_LHS 18
#define NT_RHS 19
#define NT_XRHS 20
#define NT_TERMINATE 21
#define NT_RULE 22
#define NT_XRULE 23
#define NT_GRAMMAR 24

#define TK_SETID(tk, value) do {tk.id = value; tk.class = #value;} while(0);
#define TK_SETLEX(tk, c) do {free(tk.lexeme); tk.lexeme = malloc(sizeof(char) * 2); sprintf(tk.lexeme, "%c", c);} while(0);

#define FIRST_NT NT_LHS

#define LL_TABLE \
{\
    (int[]){-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},/*NT_LHS*/\
    (int[]){-1,-1,-1, 7,-1,-1,10,-1,11,-1,12,-1, 8, 9,-1,-1,-1},/*NT_RHS*/\
    (int[]){-1,-1,-1,-1,15,14,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},/*NT_XRHS*/\
    (int[]){-1, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5},/*NT_TERMINATE*/\
    (int[]){ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},/*NT_RULE*/\
    (int[]){ 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},/*NT_XRULE*/\
    (int[]){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /*NT_GRAMMAR*/\
}

#define PRODUCTIONS \
{ \
    (int[]){NT_XRULE,UNKNOWN},                                /*0. <grammar> ::= <xrule>;*/\
    (int[]){NT_XRULE,NT_RULE,UNKNOWN},                        /*1. <xrule> ::= <rule><xrule>;*/\
    (int[]){UNKNOWN},                                         /*2. <xrule> ::= &;*/\
    (int[]){NT_TERMINATE,NT_RHS,DEFINE,NT_LHS,UNKNOWN},       /*3. <rule> ::= <lhs>"="<rhs><terminate>;*/\
    (int[]){TERMINATION_SC,UNKNOWN},                          /*4. <terminate> ::= ";";*/\
    (int[]){TERMINATION_DOT,UNKNOWN},                         /*5. <terminate> ::= ".";*/\
    (int[]){IDENTIFIER,UNKNOWN},                              /*6. <lhs> ::= identifier;*/\
    (int[]){IDENTIFIER,UNKNOWN},                              /*7. <rhs> ::= identifier;*/\
    (int[]){TERMINAL_DQ,UNKNOWN},                             /*8. <rhs> ::= terminal_dq;*/\
    (int[]){TERMINAL_SQ,UNKNOWN},                             /*9. <rhs> ::= terminal_sq;*/\
    (int[]){CLOSE_OPTION,NT_RHS,OPEN_OPTION,UNKNOWN},         /*10.<rhs> ::= "[" <rhs> "]";*/\
    (int[]){CLOSE_REPETITION,NT_RHS,OPEN_REPETITION,UNKNOWN}, /*11.<rhs> ::= "{" <rhs> "}";*/\
    (int[]){CLOSE_GROUP,NT_RHS,OPEN_GROUP,UNKNOWN},           /*12.<rhs> ::= "(" <rhs> ")";*/\
    (int[]){NT_XRHS,NT_RHS,UNKNOWN},                          /*13.<rhs> ::= <rhs><xrhs>;*/\
    (int[]){NT_RHS,UNION,UNKNOWN},                            /*14.<xrhs> ::= "|"<rhs>;*/\
    (int[]){NT_RHS,CAT,UNKNOWN}                               /*15.<xrhs> ::= "," <rhs>;*/\
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
