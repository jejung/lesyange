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

#ifndef CEXTENSIONS_H
#define CEXTENSIONS_H

#include <stdio.h>

typedef struct ilstack_node_t {
    int value;
    struct ilstack_node_t *next;
} ilstack_node_t;

typedef struct {
    ilstack_node_t *top;
    int size;
} ilstack_t;

void ilstack_push(ilstack_t *stack, int value);
int ilstack_pop(ilstack_t *stack);
int ilstack_top(ilstack_t *stack);
void ilstack_init(ilstack_t *stack);
/**
 * Allocate a new string that represents the itens on the stack separated by 
 * commas. You need to free the new allocated string when it's not more used, 
 * the size of the allocated
 * memory is the exactly size as needed.
 */
char *ilstack_toa(ilstack_t *stack);
/**
 * Append the given char to the given input. The input needs to be large enough.
 * The difference in chars from the input to the new char will be returned.
 */
int sputc(char *input, const char c);
/**
 * Append the result of transforming the i parameter in char representation
 * to the b pointer. The b pointer needs to be large enough. The count of
 * bytes written to the string will be returned, note that this canbe or not
 * the new length of the string.
 */
int itoa(int i, char *b);
/**
 * Get the size of the file and malloc sufficient space for it, then read all 
 * the content of the file in the allocated space, and return it. The actual pos
 * of the stream is maintained when this function execcutes succesfull, but if
 * one error occours, NULL will be returned and the stream pos may change.
 * You can teste ferror(file) after calling this function to know if an IO error
 * was raised.
 */
char *fcat(FILE *file);

#endif
