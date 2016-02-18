#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lesyange.h"
#include "ebnf_parser.h"
#include "cextensions.h"

void next_identifier(FILE *fp, int *col, ebnf_token_t *tk) 
{
    TK_SETID(tk, IDENTIFIER);
    while (!feof(fp)) 
    {
        int c = fgetc(fp);
        if (isalpha(c) || isdigit(c) || c == '_')
        {
            INCP(col);
        } else 
        {
            fseek(fp, -1, SEEK_CUR);
            return;
        }
    }
}

void next_comment(FILE *fp, int *line, int *col, ebnf_token_t *tk) 
{
    TK_SETID(tk, COMMENT);
    int last = 0;
    while (!feof(fp)) 
    {
        int c = fgetc(fp);
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
    UNEXPECTED_ERROR(fp, ERROR_UNTERMINATED_COMMENT, 
        "The comment at line %d, col %d was not correctly closed", 
        tk->line, tk->col);
}

void next_group_or_comment(FILE *fp, int *line, int *col, ebnf_token_t *tk) 
{
    int c = fgetc(fp);
    if (feof(fp)) 
    {
        UNEXPECTED_ERROR(fp, ERROR_UNEXPECTED_EOF, "Unexpected eof at line %d, col %d", *line, *col);
    } else if (c == '*') 
    {
        next_comment(fp, line, col, tk);
    } else 
    {
        fseek(fp, -1, SEEK_CUR);
        TK_SETID(tk, OPEN_GROUP);
    } 
}

void next_terminal(FILE *fp, int *line, int *col, const char close, ebnf_token_t *tk) 
{
    int c = fgetc(fp);
    INCP(col);
    while (!feof(fp)) 
    {
        if (c == close) 
        {
            return; 
        } else if (c == '\n')
        {
            INCP(line);
            *col = 1;
        }
        c = fgetc(fp);
        INCP(col);
    }
    UNEXPECTED_ERROR(fp, ERROR_UNTERMINATED_LITERAL, 
        "The literal at line %d, col %d was not correctly closed", tk->line, tk->col);
}

ebnf_token_t *next_token(FILE *fp, int *line, int *col) 
{
    ebnf_token_t *tk = malloc(sizeof(ebnf_token_t));
    TK_SETID(tk, UNKNOWN);
    long int spos = ftell(fp);
    int c = fgetc(fp);
    tk->lexeme = NULL; 
    tk->line = *line;
    tk->col = *col;
       
    while (!feof(fp)) 
    {
        if (isalpha(c)) 
        {
            next_identifier(fp, col, tk);
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
            TK_SETID(tk, DEFINE);
            goto bingo;
        } else if (c == ',') 
        {
            TK_SETID(tk, CAT);
            goto bingo;
        }  else if (c == ';')
        {
            TK_SETID(tk, TERMINATION_SC);
            goto bingo;
        } else if (c == '|') 
        {
            TK_SETID(tk, UNION);
            goto bingo;
        } else if (c == '[') {
            TK_SETID(tk, OPEN_OPTION);
            goto bingo;
        }  else if (c == ']') {
            TK_SETID(tk, CLOSE_OPTION);
            goto bingo;
        } else if (c == '{') {
            TK_SETID(tk, OPEN_REPETITION);
            goto bingo;
        } else if (c == '}') {
            TK_SETID(tk, CLOSE_REPETITION);
            goto bingo; 
        } else if (c == '(') {
            next_group_or_comment(fp, line, col, tk);
            if (tk->id == COMMENT) 
            {
                free(tk);
                tk = next_token(fp, line, col);
                goto bingo;
            }
            goto bingo;
        } else if (c == ')') {
            TK_SETID(tk, CLOSE_GROUP);
            goto bingo;
        } else if (c == '\"') {
            TK_SETID(tk, TERMINAL_DQ);
            next_terminal(fp, line, col, '\"', tk); 
            goto bingo;
        } else if (c == '\'') {
            TK_SETID(tk, TERMINAL_SQ);
            next_terminal(fp, line, col, '\'', tk);
            goto bingo;
        } else if (c == '<') {
            next_identifier(fp, col, tk);
            c = fgetc(fp);
            INCP(col);
            if (c != '>')
                UNEXPECTED_ERROR(fp, ERROR_UNEXPECTED_CHAR, 
                  "Expecting '<' but found '%c' at line %d, col %d", c, *line, *col);
            goto bingo;
        } else if (c == '-') {
            TK_SETID(tk, EXCEPTION);
            goto bingo;
        } else if (c == '.') {
            TK_SETID(tk, TERMINATION_DOT);
            goto bingo;
        } else 
        {
            TK_SETID(tk, UNKNOWN);
            goto bingo;
        }
        spos = ftell(fp);
        c = fgetc(fp);
        tk->line = *line;
        tk->col = INCP(col);
        if (feof(fp)) 
        {
            TK_SETID(tk, DOLLAR);
            TK_SETLEX(tk, '$');
            return tk;
        }
    }
bingo: {
    long int fpos = ftell(fp);
    fseek(fp, spos, SEEK_SET);
    unsigned int lex_len = (unsigned int)(fpos - spos);
    tk->lexeme = malloc(sizeof(char) * (lex_len + 1u));
    char *p = tk->lexeme;
    *p = '\0';
    while (lex_len--)
    {
        sputc(p++, (char) fgetc(fp));
    }
}
    return tk;
}

void parse_ebnf(OPT_CALL) 
{
    FILE *fp = fopen(opt.ebnf_file, "r+");
    int line = 1, col = 1;
    if (fp != NULL) 
    {
        int *lltable[] = LL_TABLE;
        int *productions[] = PRODUCTIONS;
        ilstack_t stack;
        ilstack_init(&stack);
        ilstack_push(&stack, DOLLAR);
        ilstack_push(&stack, NT_GRAMMAR);
        while (stack.top != NULL)
        {
            long int pos = ftell(fp);
            ebnf_token_t *tk = next_token(fp, &line, &col);
            DEBUG_LOG(opt, "Lexer: %s(%s) at line %d, col %d", tk->class, tk->lexeme, tk->line, tk->col);
            if (opt.d)
            {
                char *stack_str = ilstack_toa(&stack);
                DEBUG_LOG(opt, "Stack: %s", stack_str);
                free(stack_str);
            }
            int top = ilstack_pop(&stack);
            if (tk->id == top)
            {
                DEBUG_LOG(opt, 
                    "Syntatic: Reduce %s(%s) at line %d, col %d", tk->class, tk->lexeme, tk->line, tk->col);
            } else 
            {
                if (top < FIRST_NT)
                {
                    UNEXPECTED_ERROR(fp, ERROR_UNEXPECTED_TOKEN, 
                      "Unexpected token %s(%s) at line %d, col %d", tk->class, tk->lexeme, tk->line, tk->col);
                }
                int shift = lltable[top-FIRST_NT][tk->id];
                if (shift == -1) 
                {
                    UNEXPECTED_ERROR(fp, ERROR_UNEXPECTED_TOKEN, 
                     "Unexpected token %s(%s) at line %d, col %d", tk->class, tk->lexeme, tk->line, tk->col);
                }
                DEBUG_LOG(opt, "Syntatic: Shift %d -> %d at line %d, col %d", top, shift, tk->line, tk->col);
                int *prod = productions[shift];
                while (*prod != UNKNOWN) 
                {
                    ilstack_push(&stack, *prod);
                    prod++;
                }
                fseek(fp,pos,SEEK_SET);
            }
            free(tk);
        }    
        fclose(fp);
    } else 
    {
        UNEXPECTED_ERROR(fp, ERROR_OPENING_FILE, "Unable to open file: %s", opt.ebnf_file);
    } 
}
