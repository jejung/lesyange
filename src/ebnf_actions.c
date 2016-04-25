#include "ebnf_parser.h"
#include "lesyange.h"
#include <stdio.h>

void execute_ebnf_action(OPT_CALL, int action, ebnf_token_t *token)
{
    DEBUG_LOG(opt, 'a',
        "Semantic: Action %d @%s(%s) at line %d, col %d ",
        action, token->class, token->lexeme, token->line, token->col);
    
}
