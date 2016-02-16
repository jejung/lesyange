#ifndef CEXTENSIONS_H
#define CEXTENSIONS_H

typedef struct __ilstack_node {
    int value;
    struct __lstack_node *next;
} ilstack_node_t;

typedef struct {
    ilstack_node_t *top;
} ilstack_t;

void ilstack_push(ilstack_t *stack, int value);
int ilstack_pop(ilstack_t *stack);
int ilstack_top(ilstack_t *stack);

char *sputc(const char *input, const char c);

#endif