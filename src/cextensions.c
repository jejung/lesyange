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