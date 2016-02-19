#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cextensions.h"

int sputc(char *input, const char c)
{
    int s = 0;
    while(*input != '\0'){s++; input++;};
    *input = c;
    s++;
    input++;
    *input = '\0';
    return s;
}

int itoa(int i, char *b) 
{
    char const digit[] = "0123456789";
    char *p = b;
    int size = 0;
    if (i < 0){
        if (*p == '\0')
            *p = '-';
        else {
            *p++ = '-';
            size++;
        }
        i *= -1;
    }

    int control = i;
    do{
        ++p;
        size++;
        control = control/10;
    } while(control);

    *p = '\0';
    do{ 
        *--p = digit[i%10];
        i = i/10;
    } while(i);

    return size;
}

void ilstack_init(ilstack_t *stack)
{
    stack->top = NULL;
    stack->size = 0;
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
    stack->size--;
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
    stack->size++;
}

char *ilstack_toa(ilstack_t *stack) 
{
    ilstack_node_t *node = stack->top;
    // measure the size and keep tack of each item representation.
    char *buffer[stack->size];
    unsigned int total_len = 0;
    int nc = 0;
    while (node != NULL)
    {
        char *value = malloc(sizeof(char) * 25);
        *value = '\0';
        total_len += (unsigned int) itoa(node->value, value);       
        if (nc!=0) 
        {
            total_len++;
        }
        buffer[nc] = (char*)value;
        nc++;
        node = node->next;
    }
    char *newstr = malloc(sizeof(char) * (long unsigned int)(total_len + 1));
    int i;
    for (i = 0; i < stack->size; i++)
    {
        strcat(newstr, buffer[i]);
        if (i != nc-1)
        {
            sputc(newstr, ',');
        }
        free(buffer[i]);        
    }
    return newstr;
}

enum CEXT_ERROR_CODE cat_file(FILE *file, char **target)
{
    char *content = NULL;
    int s = fseek(file, 0, SEEK_END);
    if (s)
        return content;
    long int size = ftell(file);
    content = malloc((unsigned) (size + 1));
    if (content == NULL)
        return content;
    s = fseek(file, 0, SEEK_SET);
    if (s) {
        free(content);
        return NULL;
    }
    s = (int) fread(content, (size_t)size, 1, file);
    if (s == 0) {
        free(content);
        return NULL;
    }
    content[size] = '\0';
    return content;
}
