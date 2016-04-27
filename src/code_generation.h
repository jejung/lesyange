#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

typedef struct __non_terminal_s {
    int id;
    char *name;
    struct __non_terminal_s *next;
} non_terminal_t;

typedef struct __terminal_s {
    int id;
    char *value;
    struct __terminal_s *next;
} terminal_t;

#endif