#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lesyange.h"
#include "ebnf_parser.h"
#include "cextensions.h"

ebnf_token_t next_identifier(FILE *fp, int *line, int *col, char first) 
{
	ebnf_token_t tk;
	tk.id = IDENTIFIER;
	tk.line = *line;
	tk.col = *col;
    tk.lexeme = malloc(sizeof(char) * 2);
    sprintf(tk.lexeme, "%c", first);
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

ebnf_token_t next_token(FILE *fp, int *line, int *col) 
{
	ebnf_token_t tk;
	tk.id = UNKNOWN;
	tk.line = *line;
	tk.col = *col;
	
	int c = fgetc(fp); 
	INCP(col);
    
	while (!feof(fp)) 
    {
		if (isalpha(c)) 
        {
			return next_identifier(fp, line, col, (char)c);
		} else if (c == '\t' || c == '\r' || c == ' ') 
        {
			// nothing to do;		
		} else if (c == '\n') 
        {
			INCP(line);
			*col = 0;
		} else if (c == '=') 
        {
			tk.id = DERIVES;
            tk.lexeme = malloc(sizeof(char) * 2);
            sprintf(tk.lexeme, "%c", '=');
			tk.line = *line;
			tk.col = *col;
			return tk;
		} else 
        {
			tk.id = UNKNOWN;
			tk.lexeme = malloc(sizeof(char) * 2);
            sprintf(tk.lexeme, "%c", c);
			tk.line = *line;
			tk.col = *col;
			return tk;
		}
		c = fgetc(fp);
		INCP(col);
        if (feof(fp)) 
        {
            tk.id = DOLLAR;
            tk.lexeme = malloc(sizeof(char) * 2);
            sprintf(tk.lexeme, "%c", '$');
            tk.line = *line;
            tk.col = *col;
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
		while (!feof(fp)) 
        {
			ebnf_token_t tk = next_token(fp, &line, &col);
			if (opt.d) 
            {
				print_token(tk);
			}
		}			
		fclose(fp);
	} else 
    {
		printf("Unable to open file: %s\n", opt.ebnf_file);
		fclose(fp);
		exit(1);
	}	
}

void print_token(ebnf_token_t tk) 
{
	const char *class = "UNKNOWN";
	switch (tk.id) 
    {
        case 0: class = "EPISOLON"; break;
        case 1: class = "DOLLAR"; break;
        case 2: class = "SEMICOLON"; break;
        case 3: class = "DERIVES"; break;
        case 4: class = "TERMINAL"; break;
        case 5: class = "NONTERMINAL"; break;
        case 6: class = "RULE"; break;
        case 7: class = "IDENTIFIER"; break;
        default: break;
	}
	printf("%s(%s) at line %d, col %d\n", class, tk.lexeme, tk.line, tk.col);
}
