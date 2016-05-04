#include <stdio.h>
#include <string.h>
#include "ebnf_parser.h"
#include "lesyange.h"
#include "code_generation.h"

non_terminal_t *non_terminals;
terminal_t *terminals;

int item_id = 0;

void execute_setup() 
{
    non_terminals = NULL;
    terminals = NULL;
}

void execute_generate()
{
    
}

void make_non_terminal(non_terminal_t *data, ebnf_token_t *token)
{
    data->id = item_id++;
    int lex_len = strlen(token->lexeme) + 1;
    data->name = malloc(lex_len * sizeof(char));
    strcpy(data->name, token->lexeme);
    data->next = NULL;
}

void execute_declare(ebnf_token_t *token) 
{
    if (non_terminals == NULL)
    {
        non_terminals = malloc(sizeof(non_terminal_t));
        make_non_terminal(non_terminals, token);
    } else 
    {
        non_terminal_t *temp = non_terminals;
        while (temp->next != NULL)
        {
            if (strcmp(temp->name, token->lexeme) == 0)
            {
                UNEXPECTED_ERROR(ERROR_IDENTIFIER_REDECLARED, 
                    "The non terminal %s at line %d, col %d was already declared.", 
                    temp->name, token->line, token->col);
            }
            temp = temp->next;
        }
        non_terminal_t *newnode = malloc(sizeof(non_terminal_t));
        make_non_terminal(newnode, token);
        temp->next = newnode;
    }
}

void execute_stop_all()
{
    
}

void execute_stop_normal()
{
    
}

void execute_push_union()
{
    
}

void execute_push_cat()
{
    
}

void execute_ref_id(ebnf_token_t *token)
{
    
}

void execute_start_option() 
{
    
}

void execute_end_option() 
{
    
}

void execute_start_repetition() 
{
    
}

void execute_end_repetition()
{
    
}

void execute_start_group()
{
    
}

void execute_end_group() 
{
    
}

void execute_store_terminal()
{
    
}

void execute_ebnf_action(OPT_CALL, int action, ebnf_token_t *token)
{
    DEBUG_LOG(opt, 'a',
        "Semantic: Action %d @%s(%s) at line %d, col %d ",
        action, token->class, token->lexeme, token->line, token->col);
    switch (action) {
        case ACT_SETUP: 
            execute_setup();
            break;
        case ACT_GENERATE:
            execute_generate();
            break;
        case ACT_DECLARE:
            execute_declare(token);
            break;
        case ACT_STOP_ALL:
            execute_stop_all();
            break;
        case ACT_STOP_NORMAL:
            execute_stop_normal();
            break;
        case ACT_PUSH_UNION: 
            execute_push_union();
            break;
        case ACT_PUSH_CAT: 
            execute_push_cat();
            break;
        case ACT_REF_ID:
            execute_ref_id(token);
            break;
        case ACT_START_OPTION:
            execute_start_option();
            break;
        case ACT_END_OPTION: 
            execute_end_option();
            break;
        case ACT_START_REPETITION:
            execute_start_repetition();
            break;
        case ACT_END_REPETITION:
            execute_end_repetition();
            break;
        case ACT_START_GROUP: 
            execute_start_group();
            break;
        case ACT_END_GROUP:
            execute_end_group();
            break;
        case ACT_STORE_TERMINAL:
            execute_store_terminal(token);
            break;
    }
}
