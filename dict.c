
#include "dict.h"

// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>
// #include <stddef.h>
// #include <ctype.h>

/* dcthash: Hashes a string key.
 * NOTE: This is certainly not the best known string hashing function, but it
 *       is reasonably performant and easy to predict when testing. */
static unsigned long int dcthash(char *key) {
    unsigned long int code = 0, i;

    for (i = 0; i < 8 && key[i] != '\0'; i++) {
        code = key[i] + 31 * code;
    }

    return code;
}

/* dctcreate: Creates a new empty dictionary.
 * TODO: Implement this function. It should return a pointer to a newly
 *       dynamically allocated dictionary with an empty backing array. */

Dict *dctcreate() {
    int i;
    Dict *dict = malloc(sizeof(Dict));
    if (dict == NULL) {
        return NULL;  
    }

    dict->cap = 31;
    dict->arr = malloc(sizeof(Node*) * dict->cap);  
    if (dict->arr == NULL) {  
        free(dict);           
        return NULL;
    }

    for (i=0; i<dict->cap; i++) {
        dict->arr[i] = NULL;
    }

    dict->size = 0;           
    return dict;             
}

/* dctdestroy: Destroys an existing dictionary.
 * TODO: Implement this function. It should deallocate a dictionary, its
 *       backing array, and all of its nodes. */

void dctdestroy(Dict *dct) {
    int i;
    Node *current;
    Node *next;

    if (dct == NULL) {
        return;
    }

    for (i = 0; i < dct->cap; i++) {
        current = dct->arr[i];

        while (current != NULL) {
            /*
            printf("node to free: %s\n", current->key);
            */
            next = current->next;
            free(current->key);     
            free(current);
            current = next;
        }
    }
    free(dct->arr);
    free(dct);
}


/* dctget: Gets the value to which a key is mapped.
 * TODO: Implement this function. It should return the value to which "key" is
 *       mapped, or NULL if it does not exist. */
void *dctget(Dict *dct, char *key) {
    Node *current = NULL;
    unsigned long int hashedKey;
    unsigned long int index;

    if (dct == NULL || key == NULL){
        return NULL;
    }

    hashedKey = dcthash(key);
    index = hashedKey % (dct->cap);

    current = dct->arr[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->val; 
        }
        current = current->next;
    }

    return NULL;
}

/* Rehashes the dictionary to a new capacity */
void dctrehash(Dict *dct) {
    int oldCap = dct->cap;
    int newCap = (oldCap * 2) + 1;
    Node **newArr = (Node**)malloc(sizeof(Node*) * newCap);
    Node *current;
    Node *temp;
    int i, newIndex;

    if (newArr == NULL) {
        return;  /* Memory allocation failure */
    }

    for (i = 0; i < newCap; i++) {
        newArr[i] = NULL;
    }

    /* Transfer entries to new array */
    for (i = 0; i < oldCap; i++) {
        current = dct->arr[i];
        while (current != NULL) {
            newIndex = dcthash(current->key) % newCap;
            temp = current->next;
            current->next = newArr[newIndex];
            newArr[newIndex] = current;
            current = temp;
        }
    }

    free(dct->arr);
    dct->arr = newArr;
    dct->cap = newCap;
}

/* dctinsert: Inserts a key, overwriting any existing value.
 * TODO: Implement this function. It should insert "key" mapped to "val".
 * NOTE: Depending on how you use this dictionary, it may be useful to insert a
 *       dynamically allocated copy of "key", rather than "key" itself. Either
 *       implementation is acceptable, as long as there are no memory leaks. */
void dctinsert(Dict *dct, char *key, void *val) {
    /* int loadFactor = ((dct->size) / (dct->cap)); */
    Node *current = NULL;
    Node *newNode = NULL;
    Node *lastNode = NULL;
    unsigned long int index = 0;
    int keyLength = 0;
    char* newKey = NULL;
    unsigned long int hashedKey = 0;
    float loadFactor;

    /* Check for NULL dictionary or key */
    if (dct == NULL || key == NULL) {
        return;  
    }

    /* Calculate load factor and rehash if needed */
    loadFactor = (float)dct->size / dct->cap;
    if (loadFactor >= 1.0) {
        dctrehash(dct);
    }

    hashedKey = dcthash(key);
    index = hashedKey % (dct->cap);
    current = dct->arr[index];

    while(current != NULL){ /* while I am not at the end or the last node*/
        if(strcmp(current->key, key) == 0){ /* if the key that I want to insert already exists*/
            current->val = val; /* overwrite that value */
            return;
        }
        lastNode = current;
        current = current->next;
    }

    if (lastNode != NULL) {current = lastNode;}

    /*if out of this while loop, that means I have reached the end so add in the new node and adjust pointers */
    newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL){
        return;
    }
    keyLength = strlen(key) + 1;
    newKey = (char*)malloc(sizeof(char) * keyLength);
    if (newKey == NULL){
        return;
    }
    newKey[keyLength - 1] = '\0';
    newNode->key = newKey;
    strcpy(newNode->key, key);
    
    newNode->val = val;
    newNode->next = dct->arr[index];
    dct->arr[index] = newNode;
    dct->size++;
}




/* dctremove: Removes a key and the value to which it is mapped.
 * TODO: Implement this function. It should remove "key" and return the value
 *       to which it was mapped, or NULL if it did not exist. */
void *dctremove(Dict *dct, char *key) {
    Node *current;
    Node *prev;
    void *val;
    unsigned long int index = 0;
    unsigned long int hashedKey = 0;

    if (dct==NULL || key == NULL){
        return NULL;
    }

    hashedKey = dcthash(key);
    index = hashedKey % (dct->cap);

    prev = NULL;
    current = dct->arr[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                dct->arr[index] = current->next;
            } else {
                prev->next = current->next;
            }
            val = current->val;
            free(current->key); 
            free(current);      
            dct->size--;
            return val;         
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

int getIndex(Dict *dct, char *key) {
    unsigned long int hashedKey;
    unsigned long int index;

    hashedKey = dcthash(key);
    index = hashedKey % (dct->cap);

    return index;
}

/* dctkeys: Enumerates all of the keys in a dictionary.
 * TODO: Implement this function. It should return a dynamically allocated array
 *       of pointers to the keys, in no particular order, or NULL if empty. */
char **dctkeys(Dict *dct) {
    char **keys;
    Node *current;
    int i = 0;
    int j = 0;
    int keyCount = dct->size;

    if (dct==NULL){
        return NULL;
    }

    if (keyCount == 0){
        return NULL;
    }

    keys = (char **)malloc(sizeof(char*) * keyCount);
    if (keys == NULL) {  /*have to check if the MALLOCKING failed */
        return NULL;
    }

    for (i = 0; i < dct->cap; i++) { /*keeping track of the backing array going through indeces of that */
        current = dct->arr[i]; /*the current node is the index I am at*/

        while (current != NULL){ /* while I still have nodes*/
            keys[j] = current->key; /*go through the array I have mallocked */
            j++; /*increment that*/
            current = current->next; /*go to the next node*/
            }
        }
    return keys; 
}