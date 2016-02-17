#include <stdlib.h>
#include <string.h>

#include "cextensions.h"

char *sputc(const char *input, const char c)
{
    char *new, *ptr;
    
    new = calloc((size_t)strlen(input) + 2, sizeof(char));
    ptr = new;
    for(; *input; input++) {*ptr = *input; ptr++;}
    *ptr = c;
    
    return new;
}

void ilstack_init(ilstack_t *stack)
{
    stack->top = NULL;
}

int ilstack_top(ilstack_t *stack)
{
    return stack->top->value;
}

int ilstack_pop(ilstack_t *stack)
{
    ilstack_node_t *top = stack->top;
    stack->top = top->next;
    int val = top->value;
    free(top);
    return val;    
}

void ilstack_push(ilstack_t *stack, int value) 
{
    ilstack_node_t *newnode = malloc(sizeof(ilstack_node_t));
    newnode->value = value;
    if (stack->top == NULL)
    {   
        stack->top = newnode;
        newnode->next = NULL;
    } else 
    {
        ilstack_node_t *top = stack->top;
        newnode->next = top;
        stack->top = newnode;
    }   
}
