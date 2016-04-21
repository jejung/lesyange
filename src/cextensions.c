/*  lesyange - Lexical and Syntatic Analyzers Generator.
    Copyright (C) 2016  Jean Jung

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

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

char *fcat(FILE *file)
{
    if (fseek(file, 0, SEEK_END))
        return NULL;
    long size = ftell(file);
    char *content = calloc((size_t)size + 1, sizeof(char));
    if (content == NULL)
        return NULL;
    rewind(file);
    fread(content, (size_t)size, 1, file);
    content[size] = '\0';
    return content;
}
