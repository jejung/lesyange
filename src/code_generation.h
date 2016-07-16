#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#define RULE_TYPE_STR 1

struct __rule_s;

typedef union {
    char *str;
    struct __rule_s *group;
} rule_value_t;

typedef struct __rule_s {
    int type;
    rule_value_t value;
    struct __rule_s *next;
} rule_t;

typedef struct __non_terminal_s {
    int id;
    char *name;
    rule_t *rules;
    struct __non_terminal_s *next;
} non_terminal_t;

#endif
