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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lesyange.h"
#include "ebnf_parser.h"
#include "cextensions.h"

void next_identifier(char **psource, int *col, ebnf_token_t *tk) 
{   
    TK_SETID(tk, IDENTIFIER);
    while (*((*psource)++) != '\0') 
    {
        int c = **psource;
        if (isalpha(c) || isdigit(c) || c == '_')
        {
            INCP(col);
        }
        else
        {   
            (*psource)--;
            break ;
        }
    }
}

void next_comment(char **psource, int *line, int *col, ebnf_token_t *tk) 
{
    TK_SETID(tk, COMMENT);
    char last = 0;
    while (**psource != '\0') 
    {
        char c = *(*psource++);
        INCP(col);
        if (last == '*' && c == ')') 
            return;
        
        if (c == '\n') 
        {
            INCP(line);
            *col = 1;
        }
        last = c;
    }
    UNEXPECTED_ERROR(ERROR_UNTERMINATED_COMMENT, 
        "The comment at line %d, col %d was not correctly closed", 
        tk->line, tk->col);
}

void next_group_or_comment(char **psource,int *line,int *col,ebnf_token_t *tk) 
{
    char c = *(*psource++);
    if (c == '\0') 
    {
        UNEXPECTED_ERROR(ERROR_UNEXPECTED_EOF, 
            "Unexpected eof at line %d, col %d", *line, *col);
    } else if (c == '*') 
    {
        next_comment(psource, line, col, tk);
    } else 
    {
        (*psource)--;
        TK_SETID(tk, OPEN_GROUP);
    }
}

void next_terminal(char **psource,int *line,int *col,const char close,ebnf_token_t *tk) 
{
    char c = *(*psource++);
    INCP(col);
    while (c != '\0') 
    {
        if (c == close) 
        {
            return; 
        } else if (c == '\n')
        {
            INCP(line);
            *col = 1;
        }
        c = *(*psource++);
        INCP(col);
    }
    UNEXPECTED_ERROR(ERROR_UNTERMINATED_LITERAL, 
        "The literal at line %d, col %d was not correctly closed", tk->line, tk->col);
}

ebnf_token_t next_token(char **psource, int *line, int *col) 
{
    ebnf_token_t tk;
    TK_SETID(&tk, UNKNOWN);
    char *start = *psource;
    tk.lexeme = NULL; 
    tk.line = *line;
    tk.col = *col;
    char c = **psource;
    while (c != '\0') 
    {
        if (isalpha(c)) 
        {
            next_identifier(psource, col, &tk);
            goto bingo;
        } else if (c == '\t' || c == '\r' || c == ' ') 
        {
            // nothing to do;
        } else if (c == '\n')
        {
            INCP(line);
            *col = 1;
        } else if (c == '=') 
        {
            TK_SETID(&tk, DEFINE);
            goto bingo;
        } else if (c == ',') 
        {
            TK_SETID(&tk, CAT);
            goto bingo;
        }  else if (c == ';')
        {
            TK_SETID(&tk, TERMINATION_SC);
            goto bingo;
        } else if (c == '|') 
        {
            TK_SETID(&tk, UNION);
            goto bingo;
        } else if (c == '[') {
            TK_SETID(&tk, OPEN_OPTION);
            goto bingo;
        }  else if (c == ']') {
            TK_SETID(&tk, CLOSE_OPTION);
            goto bingo;
        } else if (c == '{') {
            TK_SETID(&tk, OPEN_REPETITION);
            goto bingo;
        } else if (c == '}') {
            TK_SETID(&tk, CLOSE_REPETITION);
            goto bingo; 
        } else if (c == '(') {
            next_group_or_comment(psource, line, col, &tk);
            if (tk.id == COMMENT) 
            {
                tk = next_token(psource, line, col);
                goto bingo;
            }
            goto bingo;
        } else if (c == ')') {
            TK_SETID(&tk, CLOSE_GROUP);
            goto bingo;
        } else if (c == '\"') {
            TK_SETID(&tk, TERMINAL_DQ);
            next_terminal(psource, line, col, '\"', &tk); 
            goto bingo;
        } else if (c == '\'') {
            TK_SETID(&tk, TERMINAL_SQ);
            next_terminal(psource, line, col, '\'', &tk);
            goto bingo;
        } else if (c == '<') {
            next_identifier(psource, col, &tk);
            c = *(*psource++);
            INCP(col);
            if (c != '>')
                UNEXPECTED_ERROR(ERROR_UNEXPECTED_CHAR, 
                  "Expecting '<' but found '%c' at line %d, col %d", c, *line, *col);
            goto bingo;
        } else if (c == '-') {
            TK_SETID(&tk, EXCEPTION);
            goto bingo;
        } else if (c == '.') {
            TK_SETID(&tk, TERMINATION_DOT);
            goto bingo;
        } else 
        {
            TK_SETID(&tk, UNKNOWN);
            goto bingo;
        }
        start = *psource;
        c = *(*psource++);
        tk.line = *line;
        tk.col = INCP(col);
        if (c == '\0') 
        {
            TK_SETID(&tk, DOLLAR);
            TK_SETLEX(&tk, '$');
            return tk;
        }
    }
bingo:{
    int size = 1;
    char *pstart = start;
    while (pstart++ != *psource){size++;}
    tk.lexeme = malloc(sizeof(char) * (unsigned)(size + 1));
    strncpy(tk.lexeme, start, (unsigned)size-1);
    tk.lexeme[size] = '\0';
}
    return tk;
}

void parse_ebnf(OPT_CALL) 
{
    FILE *fp = fopen(opt.ebnf_file, "r");
    if (fp == NULL) 
    {   
        UNEXPECTED_ERROR(ERROR_OPENING_FILE, "%s\n", opt.ebnf_file);
    }    
    char *source = fcat(fp);
    if (source == NULL)
    {
        if (ferror(fp)) 
        {
            UNEXPECTED_ERROR(ERROR_READING_FILE, 
                "Error reading the file %s\n", opt.ebnf_file);    
        } else
        {
            UNEXPECTED_ERROR(ERROR_NOT_ENOUGH_MEMORY, 
               "Insufficient memory to store the file contents: %s\n", 
               opt.ebnf_file);
        }
    }
    fclose(fp);
    DEBUG_LOG(opt, "Source:\n%s\n", source);
    char *psource = source;
    int line = 1, col = 1;
    int *lltable[] = LL_TABLE;
    int *productions[] = PRODUCTIONS;
    ilstack_t stack;
    ilstack_init(&stack);
    ilstack_push(&stack, DOLLAR);
    ilstack_push(&stack, NT_GRAMMAR);
    while (stack.top != NULL)
    {
        char *pos = psource;
        int tline = line, tcol = col;
        ebnf_token_t tk = next_token(&psource, &tline, &tcol);
        DEBUG_LOG(opt, 
            "Lexer: %s(%s) at line %d, col %d", 
                tk.class, tk.lexeme, tk.line, tk.col);
        if (opt.d)
        {
            char *stack_str = ilstack_toa(&stack);
            DEBUG_LOG(opt, "Stack: %s", stack_str);
            free(stack_str);
        }
        int top = ilstack_pop(&stack);
        if (tk.id == top)
        {
            line = tline;
            col = tcol;
            DEBUG_LOG(opt, 
                "Syntatic: Reduce %s(%s) at line %d, col %d", 
                    tk.class, tk.lexeme, tk.line, tk.col);
        } else 
        {
            if (top < FIRST_NT)
            {
                UNEXPECTED_ERROR(ERROR_UNEXPECTED_TOKEN,
                  "Unexpected token %s(%s) at line %d, col %d", 
                    tk.class, tk.lexeme, tk.line, tk.col);
            }
            int shift = lltable[top-FIRST_NT][tk.id];
            if (shift == -1) 
            {
                UNEXPECTED_ERROR(ERROR_UNEXPECTED_TOKEN, 
                     "Unexpected token %s(%s) at line %d, col %d", 
                        tk.class, tk.lexeme, tk.line, tk.col);
            }
            DEBUG_LOG(opt, 
                "Syntatic: Shift %d -> %d at line %d, col %d", 
                    top, shift, tk.line, tk.col);
            int *prod = productions[shift];
            while (*prod != UNKNOWN) 
            {
                ilstack_push(&stack, *prod);
                prod++;
            }
            psource = pos;
        }
    }
    free(source);    
}
