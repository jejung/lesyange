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

char* itoa(int i, char b[])
{
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ 
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
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

char *ilstack_toa(ilstack_t *stack) 
{
    ilstack_node_t *node = stack->top;
    char *str = malloc(sizeof(char));
    str[0] = 0;
    while (node != NULL)
    {
        char *old;
        if (node != stack->top)
        {
            old = str;
            str = sputc(old, ',');
            free(old);
        }
        char value[15];
        itoa(node->value, value);
        old = str;
        str = strcat(old, (const char*) value);
        free(old);
        node = node->next;
    }
    return str;
}