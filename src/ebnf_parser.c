#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lesyange.h"
#include "ebnf_parser.h"
#include "cextensions.h"

ebnf_token_t next_identifier(FILE *fp, int *col, ebnf_token_t tk) 
{
    TK_SETID(tk, IDENTIFIER);
    while (!feof(fp)) 
    {
        int c = fgetc(fp);
        if (isalpha(c) || isdigit(c) || c == '_')
        {
            char *old = tk.lexeme;
            tk.lexeme = sputc(old, (char)c); 
            free(old);
            INCP(col);
        } else 
        {
            fseek(fp, -1, SEEK_CUR);
            break;
        }
    }
    return tk;
}

ebnf_token_t next_comment(FILE *fp, int *line, int *col, ebnf_token_t tk) 
{
    TK_SETID(tk, COMMENT);
    int last = 0;
    while (!feof(fp)) 
    {
        int c = fgetc(fp);
        INCP(col);
        if (last == '*' && c == ')') 
            return tk;
        
        if (c == '\n') 
        {
            INCP(line);
            *col = 1;
        }   
        char *old = tk.lexeme;
        tk.lexeme = sputc(old, (char)c);
        free(old);
        last = c;
    }
    UNEXPECTED_ERROR(fp, ERROR_UNTERMINATED_COMMENT, 
        "The comment at line %d, col %d was not correctly closed", 
        tk.line, tk.col);
}

ebnf_token_t next_group_or_comment(FILE *fp, int *line, int *col, ebnf_token_t tk) 
{
    int c = fgetc(fp);
    if (feof(fp)) 
    {
        UNEXPECTED_ERROR(fp, ERROR_UNEXPECTED_EOF, "Unexpected eof at line %d, col %d", *line, *col);
    } else if (c == '*') 
    {
        return next_comment(fp, line, col, tk);
    } else 
    {
        fseek(fp, -1, SEEK_CUR);
        TK_SETID(tk, OPEN_GROUP);
        return tk;
    } 
}

ebnf_token_t next_terminal(FILE *fp, int *line, int *col, char close, ebnf_token_t tk) 
{
    int c = fgetc(fp);
    INCP(col);
    while (!feof(fp)) {
        char *old = tk.lexeme;
        tk.lexeme = sputc(old, (char)c);
        free(old);
        if (c == close) 
        {
            return tk; 
        } else if (c == '\n')
        {
            INCP(line);
            *col = 1;
        }
        c = fgetc(fp);
        INCP(col);
    }
    UNEXPECTED_ERROR(fp, ERROR_UNTERMINATED_LITERAL, 
        "The literal at line %d, col %d was not correctly closed", tk.line, tk.col);
}

ebnf_token_t next_token(FILE *fp, int *line, int *col) 
{
    ebnf_token_t tk;
    TK_SETID(tk, UNKNOWN);
    int c = fgetc(fp);
    tk.lexeme = NULL; 
    TK_SETLEX(tk, c);
    tk.line = *line;
    tk.col = *col;
       
    while (!feof(fp)) 
    {
        if (isalpha(c)) 
        {
            return next_identifier(fp, col, tk);
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
            return tk;
        } else if (c == ',') 
        {
            TK_SETID(tk, CAT);
            return tk;
        }  else if (c == ';')
        {
            TK_SETID(tk, TERMINATION_SC);
            return tk;
        } else if (c == '|') 
        {
            TK_SETID(tk, UNION);
            return tk;
        } else if (c == '[') {
            TK_SETID(tk, OPEN_OPTION);
            return tk; 
        }  else if (c == ']') {
            TK_SETID(tk, CLOSE_OPTION);
            return tk;
        } else if (c == '{') {
            TK_SETID(tk, OPEN_REPETITION);
            return tk;
        } else if (c == '}') {
            TK_SETID(tk, CLOSE_REPETITION);
            return tk;
        } else if (c == '(') {
            return next_group_or_comment(fp, line, col, tk);
        } else if (c == ')') {
            TK_SETID(tk, CLOSE_GROUP);
            return tk;
        } else if (c == '\"') {
            TK_SETID(tk, TERMINAL_DQ);
            return next_terminal(fp, line, col, '\"',  tk); 
        } else if (c == '\'') {
            TK_SETID(tk, TERMINAL_SQ);
            return next_terminal(fp, line, col, '\'', tk);
        } else if (c == '<') {
            tk = next_identifier(fp, col, tk);
            c = fgetc(fp);
            INCP(col);
            if (c != '>')
                UNEXPECTED_ERROR(fp, UNEXPECTED_CHAR, 
                  "Expecting '<' but found '%c' at line %d, col %d", c, *line, *col);
            char* old = tk.lexeme;
            tk.lexeme = sputc(old, (char)c);
            free(old);
            return tk;
        } else if (c == '-') {
            TK_SETID(tk, EXCEPTION);
            return tk;
        } else if (c == '.') {
            TK_SETID(tk, TERMINATION_DOT);
            return tk;
        } else 
        {
            TK_SETID(tk, UNKNOWN);
            return tk;
        }
        c = fgetc(fp);
        TK_SETLEX(tk, c);
        tk.line = *line;
        tk.col = INCP(col);
        if (feof(fp)) 
        {
            TK_SETID(tk, DOLLAR);
            tk.lexeme = malloc(sizeof(char) * 2);
            strcpy(tk.lexeme, "$");
            return tk;
        }
    }
  return tk;
}

void parse_ebnf(OPT_CALL) 
{
    FILE* fp = fopen(opt.ebnf_file, "r+");
    int line = 1, col = 1;
    if (fp != NULL) 
    {
        ilstack_t *stack = malloc(sizeof(ilstack_t));
        ilstack_init(stack);
        ilstack_push(stack, DOLLAR);
        ilstack_push(stack, START);
        while (!feof(fp)) 
        {
            ebnf_token_t tk = next_token(fp, &line, &col);
            DEBUG_LOG(opt, "%s(%s) at line %d, col %d", tk.class, tk.lexeme, tk.line, tk.col);
            int top = ilstack_top(stack);  

        }     
        fclose(fp);
    } else 
    {
        UNEXPECTED_ERROR(fp, ERROR_OPENING_FILE, "Unable to open file: %s", opt.ebnf_file);
    } 
}
