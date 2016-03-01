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

#ifndef EBNF_PARSER_H
#define EBNF_PARSER_H

#include "lesyange.h"

#define UNKNOWN -1 // ?

#define EPSILON 0 // ÿ
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
#define NT_XRHS 20
#define NT_TERMINATE 21
#define NT_RULE 22
#define NT_XRULE 23
#define NT_GRAMMAR 24

#define TK_SETID(tk, value) do {(tk)->id = value; (tk)->class = #value;} while(0);
#define TK_SETLEX(tk, c) do {\
    free((tk)->lexeme); \
    (tk)->lexeme = malloc(sizeof(char) * 2); \
    sprintf((tk)->lexeme, "%c", c);\
    } while(0);

#define FIRST_NT NT_LHS

#define ACT_SETUP 25
#define ACT_GENERATE 26
#define ACT_DECLARE 27
#define ACT_STOP_ALL 28 
#define ACT_STOP_NORMAL 29
#define ACT_PUSH_UNION 30
#define ACT_PUSH_CAT 31
#define ACT_REF_ID 32
#define ACT_START_OPTION 33
#define ACT_END_OPTION 34
#define ACT_START_REPETITION 35
#define ACT_END_REPETITION 36
#define ACT_START_GROUP 37
#define ACT_END_GROUP 38
#define ACT_STORE_TERMINAL 39

#define FIRST_ACTION ACT_SETUP

#define IS_NT(x) (x >= FIRST_NT && x < FIRST_ACTION)

#define LL_TABLE \
{\
    (int[]){-1,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},/*NT_LHS*/\
    (int[]){-1,-1,-1,-1, 7,-1,-1,10,-1,11,-1,12,-1, 8, 9,-1,-1,-1},/*NT_RHS*/\
    (int[]){-1,15,15,15,15,14,13,15,15,15,15,15,15,15,15,15,15,15},/*NT_XRHS*/\
    (int[]){-1,-1, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5},/*NT_TERMINATE*/\
    (int[]){ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},/*NT_RULE*/\
    (int[]){ 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},/*NT_XRULE*/\
    (int[]){ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} /*NT_GRAMMAR*/\
}

#define PRODUCTIONS \
{ \
(int[]){ACT_GENERATE,NT_XRULE,ACT_SETUP,UNKNOWN},/*0. <grammar> ::= <xrule>;*/\
(int[]){NT_XRULE,NT_RULE,UNKNOWN},/*1. <xrule> ::= <rule><xrule>;*/\
(int[]){UNKNOWN},/*2. <xrule> ::= &;*/\
(int[]){NT_TERMINATE,NT_XRHS,NT_RHS,DEFINE,NT_LHS,UNKNOWN},/*3. <rule> ::= <lhs>"="<rhs><xrhs><terminate>;*/\
(int[]){ACT_STOP_NORMAL,TERMINATION_SC,UNKNOWN},/*4. <terminate> ::= ";";*/\
(int[]){ACT_STOP_ALL,TERMINATION_DOT,UNKNOWN},/*5. <terminate> ::= ".";*/\
(int[]){ACT_REF_ID,IDENTIFIER,UNKNOWN},/*6. <lhs> ::= identifier;*/\
(int[]){ACT_DECLARE,IDENTIFIER,UNKNOWN},/*7. <rhs> ::= identifier;*/\
(int[]){ACT_STORE_TERMINAL,TERMINAL_DQ,UNKNOWN},/*8. <rhs> ::= terminal_dq;*/\
(int[]){ACT_STORE_TERMINAL,TERMINAL_SQ,UNKNOWN},/*9. <rhs> ::= terminal_sq;*/\
(int[]){ACT_END_OPTION,CLOSE_OPTION,NT_XRHS,NT_RHS,ACT_START_OPTION,OPEN_OPTION,UNKNOWN},/*10.<rhs> ::= "[" <rhs><xrhs> "]";*/\
(int[]){ACT_END_REPETITION,CLOSE_REPETITION,NT_XRHS,NT_RHS,ACT_START_REPETITION,OPEN_REPETITION,UNKNOWN},/*11.<rhs> ::= "{" <rhs><xrhs> "}";*/\
(int[]){ACT_END_GROUP,CLOSE_GROUP,NT_XRHS,NT_RHS,ACT_START_GROUP,OPEN_GROUP,UNKNOWN},/*12.<rhs> ::= "(" <rhs><xrhs> ")";*/\
(int[]){NT_XRHS,ACT_PUSH_UNION,NT_RHS,UNION,UNKNOWN},/*13.<xrhs> ::= "|"<rhs><xrhs>;*/\
(int[]){NT_XRHS,ACT_PUSH_CAT,NT_RHS,CAT,UNKNOWN},/*14.<xrhs> ::= "," <rhs><xrhs>;*/\
(int[]){UNKNOWN}/*15.<xrhs> ::= &;*/\
}

typedef struct {
	short id;
    const char* class;
	char *lexeme;
	int line;
	int col;
} ebnf_token_t;

void parse_ebnf(OPT_CALL);
void execute_ebnf_action(OPT_CALL, int action, ebnf_token_t *token); 

#endif
