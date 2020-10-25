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

node_t *top;
node_t *create_node(char *data);

int push(char *data);
int pop();
int peek();
int print_all();
int clean_stack_list();

#endif // _STACK_H_