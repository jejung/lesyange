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

typedef struct node {
    int info;
    struct node *next;
} stnode;

stnode *stack = NULL;
char *source;
int line = 1, col = 1;

void stack_put(int info) 
{
    if (stack == NULL)
    {
        stack = malloc(sizeof(stnode));
        stack->info = info;
        stack->next = NULL;
    } else 
    {
        stnode *temp = malloc(sizeof(stnode));
        temp->info = info;
        temp->next = stack;
        stack = temp;
    }
}

int stack_pop() 
{
    if (stack == NULL)
        return -1;
    else 
    {
        int info = stack->info;
        stnode *temp = stack;
        stack = stack->next;
        free(temp);
        return info;
    }
}

void next_identifier(ebnf_token_t *tk) 
{   
    TK_SETID(tk, IDENTIFIER);
    while (*source != '\0') 
    {
        if (isalpha(*source) || isdigit(*source) || *source == '_')
        {
            col++;
        }
        else
        {
            source--;
            return;
        }
        source++;
    }
}

void next_comment(OPT_CALL, ebnf_token_t *tk) 
{
    TK_SETID(tk, COMMENT);
    char last = '\0';
    while (*source != '\0') 
    {
        col++;
        if (last == '*' && *source == ')') 
            return;

        if (*source == '\n') 
        {
            line++;
            col = 1;
        }
        last = *source;
        source++;
    }
    UNEXPECTED_ERROR(opt, ERROR_UNTERMINATED_COMMENT, 
        "The comment at line %d, col %d was not correctly closed", 
        tk->line, tk->col);
}

void next_group_or_comment(OPT_CALL, ebnf_token_t *tk) 
{
    source++;
    char c = *source;
    if (c == '\0') 
    {
        UNEXPECTED_ERROR(opt, ERROR_UNEXPECTED_EOF, 
            "Unexpected eof at line %d, col %d", line, col);
    } else if (c == '*') 
    {
        next_comment(opt, tk);
    } else 
    {
        source--;
        TK_SETID(tk, OPEN_GROUP);
    }
}

void next_terminal(OPT_CALL, const char close, ebnf_token_t *tk) 
{
    col++;
    source++;
    while (*source != '\0') 
    {
        if (*source == close) 
        {
            return; 
        } else if (*source == '\n')
        {
            line++;
            col = 1;
        }
        source++;
        col++;
    }
    UNEXPECTED_ERROR(opt, ERROR_UNTERMINATED_LITERAL, 
        "The literal at line %d, col %d was not correctly closed", tk->line, tk->col);
}

void next_token(OPT_CALL, ebnf_token_t *tk) 
{
    TK_SETID(tk, UNKNOWN);
    char *start = source;
    tk->lexeme = NULL; 
    tk->line = line;
    tk->col = col;
    while (*source != '\0') 
    {
        if (isalpha(*source)) 
        {
            next_identifier(tk);
            break;
        } else if (*source == '\t' || *source == '\r' || *source == ' ') 
        {
            // nothing to do;
        } else if (*source == '\n')
        {
            line++;
            col = 1;
        } else if (*source == '=') 
        {
            TK_SETID(tk, DEFINE);
            break;
        } else if (*source == ',') 
        {
            TK_SETID(tk, CAT);
            break;
        }  else if (*source == ';')
        {
            TK_SETID(tk, TERMINATION_SC);
            break;
        } else if (*source == '|') 
        {
            TK_SETID(tk, UNION);
            break;
        } else if (*source == '[') {
            TK_SETID(tk, OPEN_OPTION);
            break;
        }  else if (*source == ']') {
            TK_SETID(tk, CLOSE_OPTION);
            break;
        } else if (*source == '{') {
            TK_SETID(tk, OPEN_REPETITION);
            break;
        } else if (*source == '}') {
            TK_SETID(tk, CLOSE_REPETITION);
            break;
        } else if (*source == '(') {
            next_group_or_comment(opt, tk);
            if (tk->id != COMMENT) 
            {
                break;
            }
        } else if (*source == ')') {
            TK_SETID(tk, CLOSE_GROUP);
            break;
        } else if (*source == '\"') {
            TK_SETID(tk, TERMINAL_DQ);
            next_terminal(opt, '\"', tk); 
            break;
        } else if (*source == '\'') {
            TK_SETID(tk, TERMINAL_SQ);
            next_terminal(opt, '\'', tk);
            break;
        } else if (*source == '<') {
            next_identifier(tk);
            source++;
            col++;
            if (*source != '>')
                UNEXPECTED_ERROR(opt, ERROR_UNEXPECTED_CHAR, 
                  "Expecting '<' but found '%c' at line %d, col %d", *source, line, col);
            break;
        } else if (*source == '-') {
            TK_SETID(tk, EXCEPTION);
            break;
        } else if (*source == '.') {
            TK_SETID(tk, TERMINATION_DOT);
            break;
        } else 
        {
            TK_SETID(tk, UNKNOWN);
            break;
        }
        source++;
        start = source;
        tk->line = line;
        tk->col = col++;
        if (*source == '\0') 
        {
            TK_SETID(tk, DOLLAR);
            TK_SETLEX(tk, '$');
            return;
        }
    }
    int size = 1;
    char *pstart = start;
    while (pstart != source){size++; pstart++;}
    tk->lexeme = calloc((size_t)size + 1, sizeof(char));
    strncpy(tk->lexeme, start, (unsigned)size);
    tk->lexeme[size] = '\0';
}

void parse_ebnf(OPT_CALL) 
{
    FILE *fp = fopen(opt.ebnf_file, "r");
    if (fp == NULL) 
    {   
        UNEXPECTED_ERROR(opt, ERROR_OPENING_FILE, "%s\n", opt.ebnf_file);
    }    
    source = fcat(fp);
    if (source == NULL)
    {
        if (ferror(fp)) 
        {
            UNEXPECTED_ERROR(opt, ERROR_READING_FILE, 
                "Error reading the file %s\n", opt.ebnf_file);    
        } else
        {
            UNEXPECTED_ERROR(opt, ERROR_NOT_ENOUGH_MEMORY, 
               "Insufficient memory to store the file contents: %s\n", 
               opt.ebnf_file);
        }
    }
    fclose(fp);
    DEBUG_LOG(opt, '*', "Source:\n%s\n", source);
    int *lltable[] = LL_TABLE;
    int *productions[] = PRODUCTIONS;
    int *prod = productions[0];
    while (*prod != UNKNOWN) 
    {
        stack_put(*prod);
        prod++;
    }
    while (stack != NULL)
    {
        char *pos = source; //keep track of initial point.
        ebnf_token_t tk;
        tk.class = NULL;
        tk.lexeme = NULL;
        next_token(opt, &tk);
        DEBUG_LOG(opt, 'l', 
            "Lexer: %s(%s) at line %d, col %d", 
                tk.class, tk.lexeme, tk.line, tk.col);
        if ((opt.flags & MSK_OPT_DEBUG) && opt.debug_type == 'S')
        {
            printf("Stack:");
            stnode *temp = stack;
            while (temp != NULL) 
            {
                printf("%d,", temp->info);
                temp = temp->next;
            }
            printf("\n");
        }
        int top = stack_pop();
        if (tk.id == top)
        {
            DEBUG_LOG(opt, 's', 
            "Syntatic: Reduce %s(%s) at line %d, col %d", 
                    tk.class, tk.lexeme, tk.line, tk.col);
            source++;
        } else if (IS_NT(top))
        {
            int shift = lltable[top-FIRST_NT][tk.id];
            if (shift == -1) 
            {
                UNEXPECTED_ERROR(opt, ERROR_UNEXPECTED_TOKEN, 
                     "Unexpected token %s(%s) at line %d, col %d", 
                        tk.class, tk.lexeme, tk.line, tk.col);
            }
            DEBUG_LOG(opt, 's',
                "Syntatic: Shift %d -> %d at line %d, col %d", 
                    top, shift, tk.line, tk.col);
            prod = productions[shift];
            while (*prod != UNKNOWN) 
            {
                stack_put(*prod);
                prod++;
            }
            source = pos;
        } else 
        {
            execute_ebnf_action(opt, top, &tk);
            source = pos;
        }
        free(tk.lexeme);
    }
}
