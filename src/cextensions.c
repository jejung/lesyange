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

char *fcat(FILE *file)
{
    if (fseek(file, 0, SEEK_END))
        return NULL;
    long size = ftell(file);
    char *content = calloc((size_t) size + 1, sizeof(char));
    if (content == NULL)
        return NULL;
    rewind(file);
    fread(content, (size_t)size, 1, file);
    content[size] = '\0';
    return content;
}
