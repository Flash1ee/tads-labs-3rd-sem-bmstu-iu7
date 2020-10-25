#ifndef _STACK_H_
#define _STACK_H_
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "retcodes.h"

typedef struct node_s
{
    char *data;
    struct node_s *next;
} node_t;

typedef struct 
{
    node_t **free_ptr;
    size_t capacity;
    size_t len;
} free_addr_t;



node_t *top;

free_addr_t *creatre_free_addr_arr();
node_t *create_node(char *data);

int push(char *data, free_addr_t *list_addr);
int pop(free_addr_t *list_addr);
int peek();
int print_all();
int clean_stack_list(free_addr_t *list_addr);
void print_free_addr(free_addr_t *list_addr);
void clean_free_addr(free_addr_t *list_addr);

#endif // _STACK_H_