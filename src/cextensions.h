#ifndef CEXTENSIONS_H
#define CEXTENSIONS_H

typedef struct ilstack_node_t 
{
    int value;
    struct ilstack_node_t *next;
} ilstack_node_t;

typedef struct 
{
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
 * the size of the allocated memory is the exactly size as needed.
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

typedef enum
{
    false,
    true
} BOOLEAN;

typedef enum 
{
    NO_ERROR = 0,
    ERROR_OPENING_FILE,
    ERROR_NOT_ENOUGH_MEMORY,
    ERROR_IO_ERROR
} CEXT_ERROR_CODE;

/**
 * Read all the contents of the file. If something was wrong, NULL will be 
 * returned. The string allocated is of the exactly size as needed, and always
 * will have the null termination char.
 */
enum CEXT_ERROR_CODE cat_file(FILE *file);

#endif
