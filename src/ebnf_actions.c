#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ebnf_parser.h"
#include "lesyange.h"
#include "code_generation.h"

non_terminal_t *non_terminals = NULL;
non_terminal_t *nt_created = NULL;

int item_id = 0;

void free_rule_list(rule_t *list)
{
    rule_t *temp = list;
    while (temp != NULL)
    {
        rule_t *old = temp;
        temp = temp->next;
        free(old);
    }
}

void free_nt_list()
{
    non_terminal_t *temp = non_terminals;
    while (temp != NULL)
    {
        free_rule_list(temp->rules);
        non_terminal_t *old = temp;
        temp = temp->next;
        free(old);
    }
}

void execute_setup() 
{
    free_nt_list();
    non_terminals = NULL;
}

void execute_generate(OPT_CALL)
{
    
}

void make_non_terminal(non_terminal_t *data, ebnf_token_t *token)
{
    data->id = item_id++;
    unsigned int lex_len = strlen(token->lexeme) + 1;
    data->name = malloc(lex_len * sizeof(char));
    strcpy(data->name, token->lexeme);
    data->next = NULL;
    data->rules = NULL;
}

void execute_declare(OPT_CALL, ebnf_token_t *token) 
{
    if (non_terminals == NULL)
    {
        non_terminals = malloc(sizeof(non_terminal_t));
        if (non_terminals == NULL)
        {
            UNEXPECTED_ERROR(opt, ERROR_NOT_ENOUGH_MEMORY,
                "Not enough memory for operation");
        }
        make_non_terminal(non_terminals, token);
        nt_created = non_terminals;
    } else 
    {
        non_terminal_t *temp = non_terminals;
        non_terminal_t *last = temp;
        while (temp != NULL)
        {
            if (strcmp(temp->name, token->lexeme) == 0)
            {
                UNEXPECTED_ERROR(opt, ERROR_IDENTIFIER_REDECLARED, 
                    "The non terminal %s at line %d, col %d was already declared.", 
                    temp->name, token->line, token->col);
            }
            last = temp;
            temp = temp->next;
        }
        non_terminal_t *newnode = malloc(sizeof(non_terminal_t));
        if (newnode == NULL)
        {
            UNEXPECTED_ERROR(opt, ERROR_NOT_ENOUGH_MEMORY,
                "Not enough memory for operation");
        }
        make_non_terminal(newnode, token);
        last->next = newnode;
        nt_created = newnode;
    }
}

void execute_stop_all(OPT_CALL)
{
    
}

void execute_stop_normal(OPT_CALL)
{
    
}

void execute_push_union(OPT_CALL)
{
    
}

void execute_push_cat(OPT_CALL)
{
    
}

void execute_ref_id(OPT_CALL, ebnf_token_t *token)
{
    
}

void execute_start_option(OPT_CALL) 
{
    
}

void execute_end_option(OPT_CALL) 
{
    
}

void execute_start_repetition(OPT_CALL) 
{
    
}

void execute_end_repetition(OPT_CALL)
{
    
}

void execute_start_group(OPT_CALL)
{
    
}

void execute_end_group(OPT_CALL) 
{
    
}

void make_rule_terminal(OPT_CALL, rule_t *rule, ebnf_token_t *token)
{
    rule->type = RULE_TYPE_STR;
    size_t lex_len = strlen(token->lexeme) - 1;
    rule->value.str = malloc(lex_len * sizeof(char));
    char *lex = token->lexeme + 1;
    char *str = rule->value.str;
    while (*lex)
    {
        *str = *lex;
        str++; lex++; 
    }
    rule->value.str[lex_len-1] = '\0';
    rule->next = NULL;
}

void execute_store_terminal(OPT_CALL, ebnf_token_t *token)
{
    rule_t *rule = malloc(sizeof(rule_t));
    make_rule_terminal(opt, rule, token);
    if (nt_created->rules == NULL)
    {
        nt_created->rules = rule;
    } else 
    {
        rule_t *temp = nt_created->rules;
        rule_t *old = temp;
        while (temp != NULL)
        {
            old = temp;
            temp = temp->next;  
        }
        old->next = rule;
    }
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
            execute_generate(opt);
            break;
        case ACT_DECLARE:
            execute_declare(opt, token);
            break;
        case ACT_STOP_ALL:
            execute_stop_all(opt);
            break;
        case ACT_STOP_NORMAL:
            execute_stop_normal(opt);
            break;
        case ACT_PUSH_UNION: 
            execute_push_union(opt);
            break;
        case ACT_PUSH_CAT: 
            execute_push_cat(opt);
            break;
        case ACT_REF_ID:
            execute_ref_id(opt, token);
            break;
        case ACT_START_OPTION:
            execute_start_option(opt);
            break;
        case ACT_END_OPTION: 
            execute_end_option(opt);
            break;
        case ACT_START_REPETITION:
            execute_start_repetition(opt);
            break;
        case ACT_END_REPETITION:
            execute_end_repetition(opt);
            break;
        case ACT_START_GROUP: 
            execute_start_group(opt);
            break;
        case ACT_END_GROUP:
            execute_end_group(opt);
            break;
        case ACT_STORE_TERMINAL:
            execute_store_terminal(opt, token);
            break;
    }
}
