#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>


/* Node: Represents one key-value pair. */
typedef struct Node {
    char *key;
    void *val;
    struct Node *next;
} Node;

/* Dict: Represents a collection of key-value pairs. */
typedef struct Dict {
    int cap;
    Node **arr;
    int size;
} Dict;

Dict *dctcreate();
void dctdestroy(Dict *);
void *dctget(Dict *, char *);
void dctinsert(Dict *, char *, void *);
void *dctremove(Dict *, char *);
char **dctkeys(Dict *);
int getIndex(Dict *, char *);

#endif